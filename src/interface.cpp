/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 *
 *  interface.cpp
 *  For wrappers to the Qt internals.
 *
 *  To help reduce reliance on Qt, only the functions wrap the
 *  Qt functions have a wrapper here. Ideally we could move some
 *  of the Qt headers here.
 *
 *  ------------------------------------------------------------
 *
 *  The tree walking, merging, freeing and otherwise altering functions
 *  are an attempt to create a solid foundation for the internal scripting
 *  language by having a fully abstract data model similar to an AST for
 *  the whole state of the program at run-time.
 */

#include "embroidermodder.h"

#include <time.h>

bool test_program = false;

/* Make the translation function global in scope. */
QString
translate_str(const char *str)
{
    return _mainWin->tr(str);
}

const char *
translate(char *str)
{
    return _mainWin->tr(str).toStdString().c_str();
}

/* Tokenize our command using a 1 character deliminator. */
int
tokenize(char **argv, char *str, const char delim)
{
    int argc = 0;
    argv[argc] = 0;
    if (strlen(str) == 0) {
        return 0;
    }
    for (int i=0; str[i]; i++) {
        if (str[i] == delim) {
            str[i] = 0;
            argc++;
            argv[argc] = str+i;
        }
    }
    return argc;
}

/* Convert an EmbVector to a QPointF. */
QPointF
to_QPointF(EmbVector a)
{
    QPointF result(a.x, a.y);
    return result;
}

/* Convert a QPointF to an EmbVector. */
EmbVector
to_EmbVector(QPointF a)
{
    EmbVector v;
    v.x = a.x();
    v.y = a.y();
    return v;
}

/* Debug message wrapper for qDebug.
 *
 * For debugging code running on other machines append these messages to log
 * file.
 *
 * Timestamps are added to each message so we can trace in what order things
 * happen.
 */
void
debug_message(std::string msg)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    qDebug(msg.c_str());
    FILE *f = fopen("debug.txt", "a");
    fprintf(f, "%.2ld:%.2ld:%.2ld.%.3ld> %s\n",
        (ts.tv_sec/3600)%24, (ts.tv_sec%3600)/60, ts.tv_sec%60, ts.tv_nsec%1000, msg.c_str());
    fclose(f);
}

/* Utility function for add_to_path. */
void
get_n_reals(float result[], char *argv[], int n, int offset)
{
    for (int i=0; i<n; i++) {
        result[i] = atof(argv[offset+i]);
    }
}

/* Render an SVG-like .
 *
 */
QPainterPath
add_to_path(QPainterPath path, EmbVector scale, std::string command)
{
    float r[10];
    char *argv[100];
    char c[MAX_STRING_LENGTH];
    strcpy(c, command.c_str());
    int argc = tokenize(argv, c, ' ');
    for (int i=0; i<argc; i++) {
        if (!strcmp(argv[i], "M")) {
            get_n_reals(r, argv, 2, i+1);
            path.moveTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (!strcmp(argv[i], "L")) {
            get_n_reals(r, argv, 2, i+1);
            path.lineTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (!strcmp(argv[i], "A")) {
            get_n_reals(r, argv, 6, i+1);
            path.arcTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4], r[5]);
        }
        else if (!strcmp(argv[i], "AM")) {
            get_n_reals(r, argv, 5, i+1);
            path.arcMoveTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4]);
        }
        else if (!strcmp(argv[i], "E")) {
            get_n_reals(r, argv, 4, i+1);
            path.addEllipse(QPointF(r[0], r[1]), r[2], r[3]);
        }
    }
    return path;
}

/* Set whether the of parent's object that has the name given by key
 * is enabled.
 *
 * TODO: error reporting.
 */
void
set_enabled(QObject* parent, const char *key, bool enabled)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "comboBox", 8)) {
        QComboBox* comboBox = parent->findChild<QComboBox*>(key);
        if (comboBox) {
            comboBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "checkBox", 8)) {
        QCheckBox* checkBox = parent->findChild<QCheckBox*>(key);
        if (checkBox) {
            checkBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "button", 6)) {
        QPushButton* button = parent->findChild<QPushButton*>(key);
        if (button) {
            button->setEnabled(enabled);
        }
        return;
    }
}

/* Set visibility of parent's object that has the name given by key to
 * the boolean value in visibility.
 *
 * TODO: error reporting.
 */
void
set_visibility(QObject* parent, const char *key, bool visibility)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
        }
        return;
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
        }
        return;
    }
}

/* Turn our own markup in config.toml into the various UI elements.
 *
 * The required variable "type" is taken to choose a function to process
 * the UI element.
 *
 * This would be faster as a switch table.
 *
 * This function should take a parent object to build...
 */
void
make_ui_element(
    std::string element_type
)
{
    if (element_type == "groupBox") {

    }
    else if (element_type == "lineEdit") {

    }
    else if (element_type == "checkBox") {
        // make_checkbox(gb, description, label, icon, key);
    }
    else if (element_type == "spinBox") {

    }
    else if (element_type == "doubleSpinBox") {
        // make_spinbox(gb, description, object_name, single_step, lower, upper, key)
    }
    else if (element_type == "label") {

    }
    else if (element_type == "comboBox") {

    }
    else if (element_type == "toolButton") {

    }
    else if (element_type == "action") {

    }
    else if (element_type == "toolbar") {

    }
    else if (element_type == "menu") {

    }
}

/* . */
QCheckBox *
make_checkbox(
    QGroupBox *gb,
    std::string dictionary,
    const char *label,
    const char *icon,
    int key)
{
    QCheckBox *checkBox = new QCheckBox(translate_str(label), gb);
    checkBox->setIcon(_mainWin->create_icon(icon));
    /* checkBox->setName(); */
    if (dictionary == "settings") {
        checkBox->setChecked(settings[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { settings[key].i = x; });
    }
    if (dictionary == "dialog") {
        checkBox->setChecked(dialog[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { dialog[key].i = x; });
    }
/*    if (dictionary == "config") {
        checkBox->setChecked(config[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { config[key].i = x; });
    }
*/
    return checkBox;
}

/* TODO: Make spinbox using this toml syntax:
 *
 *     [zoom_level_spinbox]
 *     type = "doubleSpinBox"
 *     object_name = "Zoom Level"
 *     single_step = 0.1
 *     lower_bound = -10
 *     upper_bound = 10
 *     key = "settings.zoom_level"
 *
 * The content (not including):
 *
 *     QDoubleSpinBox *sb = make_spinbox(gb, desc);
 */
QDoubleSpinBox *
make_spinbox(QGroupBox *gb, std::string dictionary,
    QString object_name, EmbReal single_step, EmbReal lower, EmbReal upper,
    int key)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(gb);
    spinBox->setObjectName(object_name);
    spinBox->setSingleStep(single_step);
    spinBox->setRange(lower, upper);
    if (dictionary == "settings") {
        spinBox->setValue(settings[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ settings[key].r = (EmbReal)x; });
    }
    if (dictionary == "dialog") {
        spinBox->setValue(dialog[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ dialog[key].r = (EmbReal)x; });
    }
/*
    if (dictionary == "config") {
        spinBox->setValue(config[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ config[key].r = (EmbReal)x; });
    }
*/
    return spinBox;
}

/* Create the image widget for filename. */
ImageWidget::ImageWidget(QString filename, QWidget* parent) : QWidget(parent)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/* Load image to widget. */
bool
ImageWidget::load(QString fileName)
{
    img.load(fileName);
    return true;
}

/* Save image from widget. */
bool
ImageWidget::save(QString fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/* Destroy image widget. */
ImageWidget::~ImageWidget()
{
    debug_message("ImageWidget Destructor");
}

/* Paint the widget. */
void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

/* Create the Layer Manager. */
LayerManager::LayerManager(QWidget* parent) : QDialog(parent)
{
    layerModel = new QStandardItemModel(0, 8, this);

    layerModelSorted = new QSortFilterProxyModel;
    layerModelSorted->setDynamicSortFilter(true);
    layerModelSorted->setSourceModel(layerModel);

    treeView = new QTreeView;
    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(layerModelSorted);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeView);
    setLayout(mainLayout);

    setWindowTitle(tr("Layer Manager"));
    setMinimumSize(750, 550);

    layerModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    layerModel->setHeaderData(1, Qt::Horizontal, tr("Visible"));
    layerModel->setHeaderData(2, Qt::Horizontal, tr("Frozen"));
    layerModel->setHeaderData(3, Qt::Horizontal, tr("Z Value"));
    layerModel->setHeaderData(4, Qt::Horizontal, tr("Color"));
    layerModel->setHeaderData(5, Qt::Horizontal, tr("Linetype"));
    layerModel->setHeaderData(6, Qt::Horizontal, tr("Lineweight"));
    layerModel->setHeaderData(7, Qt::Horizontal, tr("Print"));

    addLayer("0", true, false, 0.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("1", true, false, 1.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("2", true, false, 2.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("3", true, false, 3.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("4", true, false, 4.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("5", true, false, 5.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("6", true, false, 6.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("7", true, false, 7.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("8", true, false, 8.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("9", true, false, 9.0, qRgb(0,0,0), "Continuous", "Default", true);

    for (int i = 0; i < layerModel->columnCount(); ++i)
        treeView->resizeColumnToContents(i);

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* Destroy the Layer Manager. */
LayerManager::~LayerManager()
{
    QApplication::restoreOverrideCursor();
}

QPixmap
make_pixmap(int size, int color)
{
    QPixmap pix(QSize(size, size));
    pix.fill(QColor(color));
    return pix;
}

/* Add layer to Layer Manager. */
void
LayerManager::addLayer(
    QString name,
    const bool visible,
    const bool frozen,
    const EmbReal zValue,
    const QRgb color,
    QString lineType,
    QString lineWeight,
    const bool print)
{
    layerModel->insertRow(0);
    layerModel->setData(layerModel->index(0, 0), name);
    layerModel->setData(layerModel->index(0, 1), visible);
    layerModel->setData(layerModel->index(0, 2), frozen);
    layerModel->setData(layerModel->index(0, 3), zValue);

    QPixmap colorPix = make_pixmap(16, color);
    layerModel->itemFromIndex(layerModel->index(0, 4))->setIcon(QIcon(colorPix));
    layerModel->setData(layerModel->index(0, 4), QColor(color));

    layerModel->setData(layerModel->index(0, 5), lineType);
    layerModel->setData(layerModel->index(0, 6), lineWeight);
    layerModel->setData(layerModel->index(0, 7), print);
}


SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    //Default values
    setColors(QColor(Qt::darkGreen), QColor(Qt::green), QColor(Qt::darkBlue), QColor(Qt::blue), 32);
}

/* . */
void
SelectBox::setDirection(int dir)
{
    if (!dir) {
        dirPen = leftPen;
        dirBrush = leftBrush;
    }
    else {
        dirPen = rightPen;
        dirBrush = rightBrush;
    }
    boxDir = dir;
}

/* . */
void
SelectBox::setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha)
{
    qDebug("SelectBox setColors()");
    alpha = newAlpha;

    leftPenColor = colorL; //TODO: allow customization
    leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR; //TODO: allow customization
    rightBrushColor = QColor(fillR.red(), fillR.green(), fillR.blue(), alpha);

    leftPen.setColor(leftPenColor);
    leftPen.setStyle(Qt::DashLine);
    leftBrush.setStyle(Qt::SolidPattern);
    leftBrush.setColor(leftBrushColor);

    rightPen.setColor(rightPenColor);
    rightPen.setStyle(Qt::SolidLine);
    rightBrush.setStyle(Qt::SolidPattern);
    rightBrush.setColor(rightBrushColor);

    setDirection(boxDir);

    forceRepaint();
}

void SelectBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

void SelectBox::forceRepaint()
{
    //HACK: Take that QRubberBand!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

/*
 */
PreviewDialog::PreviewDialog(QWidget* parent, QString caption, QString dir,
                             QString filter) : QFileDialog(parent, caption, dir, filter)
{
    qDebug("PreviewDialog Constructor");

    //TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    //TODO: make thumbnail size adjustable thru settings dialog
    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay)) {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    //TODO: connect the currentChanged signal to update the preview imgWidget.
}

/*
 */
PreviewDialog::~PreviewDialog()
{
    qDebug("PreviewDialog Destructor");
}


/* . */
UndoEditor::UndoEditor(QString  iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(tr("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

/* Destroy the undo editor, maintaining the stack. */
UndoEditor::~UndoEditor()
{
}

/* . */
void
UndoEditor::updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

/* . */
void
UndoEditor::addStack(QUndoStack* stack)
{
    undoGroup->addStack(stack);
}

/* . */
bool
UndoEditor::canUndo()
{
    return undoGroup->canUndo();
}

/* . */
bool
UndoEditor::canRedo()
{
    return undoGroup->canRedo();
}

/* . */
QString
UndoEditor::undoText()
{
    return undoGroup->undoText();
}

/* . */
QString
UndoEditor::redoText()
{
    return undoGroup->redoText();
}

/* . */
void
UndoEditor::undo()
{
    undoGroup->undo();
}

/* . */
void
UndoEditor::redo()
{
    undoGroup->redo();
}


StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    int n = string_array_length(button_list);
    for (int i=0; i<n; i++) {
        QString name = "StatusBarButton";
        QToolButton* button = new QToolButton(parent);
        button->setObjectName(name + button_list[i]);

        button->setText(QString::fromStdString(button_list[i]));
        button->setAutoRaise(true);
        button->setCheckable(true);

        connect(button, &QToolButton::toggled, this, [=](bool a) { toggle(button_list[i], a); } );
        buttons[button_list[i]] = button;
    }

    statusBarMouseCoord = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    for (int i=0; i<n; i++) {
        this->addWidget(buttons[button_list[i]]);
    }
}

void StatusBar::setMouseCoord(EmbReal x, EmbReal y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/**
 * .
 */
void
StatusBar::context_menu_action(QToolButton *button, const char *icon, const char *label, QMenu *menu, std::string setting_page)
{
    QAction* action = new QAction(_mainWin->create_icon(icon), label, menu);
    connect(action, &QAction::triggered, button, [=](){
        char cmd[MAX_STRING_LENGTH];
        strcpy(cmd, ("settingsdialog " + setting_page).c_str());
        actuator(cmd);
    });
    menu->addAction(action);
}

/**
 * .
 */
void
StatusBar::context_menu_event(QContextMenuEvent *event, QToolButton *button)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(button);
    QString name = button->objectName();
    if (name == "StatusBarButtonSNAP") {
        context_menu_action(button, "gridsnapsettings", "&Settings...", &menu, "Snap");
    }
    else if (name == "StatusBarButtonGRID") {
        context_menu_action(button, "gridsettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (name == "StatusBarButtonRULER") {
        context_menu_action(button, "rulersettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (name == "StatusBarButtonORTHO") {
        context_menu_action(button, "orthosettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (name == "StatusBarButtonPOLAR") {
        context_menu_action(button, "polarsettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (name == "StatusBarButtonQSNAP") {
        context_menu_action(button, "qsnapsettings", "&Settings...", &menu, "QuickSnap");
    }
    else if (name == "StatusBarButtonQTRACK") {
        context_menu_action(button, "qtracksettings", "&Settings...", &menu, "QuickTrack");
    }
    else if (name == "StatusBarButtonLWT") {
        View* gview = activeView();
        if (gview) {
            QAction* enableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender On", &menu);
            QAction* disableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender Off", &menu);

            enableRealAction->setEnabled(!(gview->state & VIEW_STATE_REAL));
            disableRealAction->setEnabled(gview->state & VIEW_STATE_REAL);

            menu.addAction(enableRealAction);
            menu.addAction(disableRealAction);

            /*
            QObject::connect(enableRealAction, &(QToolButton::triggered),
               this,  [=](){ gview->setFlag(VIEW_STATE_REAL); });

            QObject::connect(disableRealAction, &QToolButton::triggered,
                this, [=](){ gview->unsetFlag(VIEW_STATE_REAL); });
            */
        }

        context_menu_action(button, "lineweightsettings", "&Settings...", &menu, "LineWeight");
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void
StatusBar::toggle(std::string key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    View* gview = activeView();
    if (!gview) {
        return;
    }
    uint64_t flag = VIEW_STATE_GRID;
    if (key == "SNAP") {
        flag = VIEW_STATE_SNAP;
    }
    else if (key == "GRID") {
        flag = VIEW_STATE_GRID;
    }
    else if (key == "RULER") {
        flag = VIEW_STATE_RULER;
    }
    else if (key == "ORTHO") {
        flag = VIEW_STATE_ORTHO;
    }
    else if (key == "POLAR") {
        flag = VIEW_STATE_POLAR;
    }
    else if (key == "QSNAP") {
        flag = VIEW_STATE_QSNAP;
    }
    else if (key == "QTRACK") {
        flag = VIEW_STATE_QTRACK;
    }
    else if (key == "LWT") {
        flag = VIEW_STATE_LWT;
    }

    if (on) {
        gview->setFlag(flag);
    }
    else {
        gview->unsetFlag(flag);
    }
}

/* Create command prompt object. */
CmdPrompt::CmdPrompt(QWidget* parent) : QWidget(parent)
{
    debug_message("CmdPrompt Constructor");
    setObjectName("Command Prompt");

    promptInput = new CmdPromptInput(this);
    promptHistory = new CmdPromptHistory();
    promptDivider = new QFrame(this);
    promptVBoxLayout = new QVBoxLayout(this);

    promptSplitter = new CmdPromptSplitter(this);

    this->setFocusProxy(promptInput);
    promptHistory->setFocusProxy(promptInput);

    promptDivider->setLineWidth(1);
    promptDivider->setFrameStyle(QFrame::HLine);
    promptDivider->setMaximumSize(QWIDGETSIZE_MAX, 1);

    promptVBoxLayout->addWidget(promptSplitter);
    promptVBoxLayout->addWidget(promptHistory);
    promptVBoxLayout->addWidget(promptDivider);
    promptVBoxLayout->addWidget(promptInput);

    promptVBoxLayout->setSpacing(0);
    promptVBoxLayout->setContentsMargins(0,0,0,0);

    this->setLayout(promptVBoxLayout);

    for (int i=0; i<7; i++) {
        QString s(default_prompt_style[2*i+1]);
        styleHash[i] = s;
    }

    updateStyle();

    blinkState = false;
    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, [=](void) { blinkState = !blinkState; } );

    this->show();

    connect(promptInput, SIGNAL(stopBlinking()), this, SLOT(stopBlinking()));
    connect(promptInput, SIGNAL(appendHistory(QString,int)), promptHistory, SLOT(appendHistory(QString,int)));
    connect(this, SIGNAL(appendTheHistory(QString,int)), promptHistory, SLOT(appendHistory(QString,int)));

    //For use outside of command prompt
    connect(promptInput, SIGNAL(startCommand(QString)), this, SIGNAL(startCommand(QString)));
    connect(promptInput, SIGNAL(runCommand(QString, QString)), this, SIGNAL(runCommand(QString, QString)));
    connect(promptInput, SIGNAL(showSettings()), this, SIGNAL(showSettings()));

    connect(promptHistory, SIGNAL(historyAppended(QString)), this, SIGNAL(historyAppended(QString)));
}

/* floatingChanged isFloating */
void
CmdPrompt::floatingChanged(bool isFloating)
{
    qDebug("CmdPrompt floatingChanged(%d)", isFloating);
    if (isFloating) {
        promptSplitter->hide();
    }
    else {
        promptSplitter->show();
    }
}

/* saveHistory fileName html */
void
CmdPrompt::saveHistory(QString  fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    //TODO: save during input in case of crash
    QTextStream output(&file);
    if (html) {
        output << promptHistory->toHtml();
    }
    else {
        output << promptHistory->toPlainText();
    }
}

/* CmdPrompt::alert
 * txt
 */
void
CmdPrompt::alert(QString txt)
{
    QString alertTxt = "<font color=\"red\">" + txt + "</font>"; //TODO: Make the alert color customizable
    setPrefix(alertTxt);
    appendHistory(QString());
}

/* Start blinking the prompt cursor. */
void
CmdPrompt::startBlinking()
{
    blinkTimer->start(750);
    promptInput->isBlinking = true;
}

/* Stop blinking the prompt cursor. */
void
CmdPrompt::stopBlinking()
{
    blinkTimer->stop();
    promptInput->isBlinking = false;
}

/* Set prompt text color. */
void
CmdPrompt::setPromptTextColor(const QColor& color)
{
    styleHash[CONSOLE_STYLE_COLOR] = color.name();
    styleHash[CONSOLE_STYLE_SELECTION_COLOR] = color.name();
    updateStyle();
}

/* Set prompt background color. */
void
CmdPrompt::setPromptBackgroundColor(const QColor& color)
{
    styleHash[CONSOLE_STYLE_BG_COLOR] = color.name();
    styleHash[CONSOLE_STYLE_SELECTION_BG_COLOR] = color.name();
    updateStyle();
}

/* Set prompt font family. */
void
CmdPrompt::setPromptFontFamily(QString family)
{
    styleHash[CONSOLE_STYLE_FONT_FAMILY] = family;
    updateStyle();
}

/* Set prompt font style. */
void
CmdPrompt::setPromptFontStyle(QString style)
{
    styleHash[CONSOLE_STYLE_FONT_STYLE] = style;
    updateStyle();
}

/* Set prompt font size. */
void
CmdPrompt::setPromptFontSize(int size)
{
    styleHash[CONSOLE_STYLE_FONT_SIZE] = QString().setNum(size).append("px");
    updateStyle();
}

/* Update style. */
void
CmdPrompt::updateStyle()
{
    QString style = "QTextBrowser,QLineEdit{";
    for (int i=0; i<7; i++) {
        QString key(default_prompt_style[2*i]);
        style += key + ":" + styleHash[i] + ";";
    }
    style += "}";

    this->setStyleSheet(style);
}

/* appendHistory txt */
void
CmdPrompt::appendHistory(QString  txt)
{
    if (txt.isNull()) {
        emit appendTheHistory(promptInput->curText, promptInput->prefix.length());
        return;
    }
    debug_message("CmdPrompt - appendHistory()");
    emit appendTheHistory(txt, promptInput->prefix.length());
}

/* Set prefix to txt. */
void
CmdPrompt::setPrefix(QString txt)
{
    promptInput->prefix = txt;
    promptInput->curText = txt;
    promptInput->setText(txt);
}

/* CmdPromptSplitter parent */
CmdPromptSplitter::CmdPromptSplitter(QWidget* parent) : QSplitter(parent)
{
    debug_message("CmdPromptSplitter Constructor");
    setObjectName("Command Prompt Splitter");

    setOrientation(Qt::Vertical);
    //NOTE: Add two empty widgets just so we have a handle to grab
    addWidget(new QWidget(this)); addWidget(new QWidget(this));

    connect(this, SIGNAL(pressResizeHistory(int)),   parent, SLOT(startResizingTheHistory(int)));
    connect(this, SIGNAL(releaseResizeHistory(int)), parent, SLOT(stopResizingTheHistory(int)));
    connect(this, SIGNAL(moveResizeHistory(int)),    parent, SLOT(resizeTheHistory(int)));
}

/* Create handle for this command prompt. */
QSplitterHandle* CmdPromptSplitter::createHandle()
{
    return new CmdPromptHandle(orientation(), this);
}

/* Create command prompt handle object. */
CmdPromptHandle::CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent) : QSplitterHandle(orientation, parent)
{
    debug_message("CmdPromptHandle Constructor");
    setObjectName("Command Prompt Handle");

    connect(this, SIGNAL(handlePressed(int)),  parent, SIGNAL(pressResizeHistory(int)));
    connect(this, SIGNAL(handleReleased(int)), parent, SIGNAL(releaseResizeHistory(int)));
    connect(this, SIGNAL(handleMoved(int)),    parent, SIGNAL(moveResizeHistory(int)));
}

/* Process mouse press in the command prompt handle context. */
void
CmdPromptHandle::mousePressEvent(QMouseEvent* e)
{
    pressY = e->globalPosition().y();
    emit handlePressed(pressY);
}

/* Process mouse release in the command prompt handle context. */
void
CmdPromptHandle::mouseReleaseEvent(QMouseEvent* e)
{
    releaseY = e->globalPosition().y();
    emit handleReleased(releaseY);
}

/* Process mouse move in the command prompt handle context. */
void
CmdPromptHandle::mouseMoveEvent(QMouseEvent* e)
{
    moveY = e->globalPosition().y();
    int dY = moveY - pressY;
    emit handleMoved(dY);
}

/* CmdPromptHistory */
CmdPromptHistory::CmdPromptHistory(QWidget* parent) : QTextBrowser(parent)
{
    debug_message("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 57; // 19*3 (approximately three lines of text)

    this->setFrameStyle(QFrame::NoFrame);
    this->setMaximumHeight(initHeight);
    this->setMinimumWidth(200); // TODO: use float/dock events to set minimum size so when floating, it isn't smooshed.

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

/* ApplyFormatting txt prefixLength */
QString CmdPromptHistory::applyFormatting(QString  txt, int prefixLength)
{
    QString prefix = txt.left(prefixLength);
    QString usrtxt = txt.right(txt.length()-prefixLength);

    int start = -1;
    int stop = -1;

    //Bold Prefix
    prefix.prepend("<b>");
    prefix.append("</b>");

    //Keywords
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop) {
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == ']') {
                prefix.insert(i, "</font>");
            }
            if (prefix.at(i) == '[') {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
            }
            if (prefix.at(i) == '/') {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
                prefix.insert(i, "</font>");
            }
        }
    }

    //Default Values
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if (start != -1 && stop != -1 && start < stop) {
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == '}') {
                prefix.insert(i, "</font>");
            }
            if (prefix.at(i) == '{') {
                prefix.insert(i+1, "<font color=\"#00AA00\">");
            }
        }
    }

    return prefix + usrtxt;
}

/* CmdPromptHistory::appendHistory
 * txt
 * prefixLength
 */
void
CmdPromptHistory::appendHistory(QString txt, int prefixLength)
{
    QString formatStr = applyFormatting(txt, prefixLength);
    this->append(formatStr);
    emit historyAppended(formatStr);
    this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* Resize history to "y" lines. */
void
CmdPromptHistory::resizeHistory(int y)
{
    int newHeight = tmpHeight - y;
    if (newHeight < 0) {
        newHeight = 0;
    }
    setMaximumHeight(newHeight);
}

/* contextMenuEvent event */
void
CmdPromptHistory::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();
    //TODO: Extra stuff
    menu->exec(event->globalPos());
    delete menu;
}

/**
 * @brief CmdPromptInput::CmdPromptInput
 * @param parent
 */
CmdPromptInput::CmdPromptInput(QWidget* parent) : QLineEdit(parent)
{
    debug_message("CmdPromptInput Constructor");
    setObjectName("Command Prompt Input");

    defaultPrefix = tr("Command: ");
    prefix = defaultPrefix;
    curText = prefix;

    lastCmd = "help";
    curCmd = "help";
    cmdActive = false;

    rapidFireEnabled = false;

    isBlinking = false;

    this->setText(prefix);
    this->setFrame(false);
    this->setMaxLength(266);
    this->setMaximumSize(5000, 25);
    this->setDragEnabled(false);

    connect(this, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(checkCursorPosition(int,int)));
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(checkEditedText(QString)));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkChangedText(QString)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelection()));

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);

    applyFormatting();
}

/* EndCommand */
void
CmdPromptInput::endCommand()
{
    debug_message("CmdPromptInput endCommand");
    lastCmd = curCmd;
    cmdActive = false;
    rapidFireEnabled = false;
    emit stopBlinking();

    prefix = defaultPrefix;
    clear();
}

/* ProcessInput */
void
CmdPromptInput::processInput()
{
    debug_message("CmdPromptInput::processInput");

    updateCurrentText(curText);

    QString cmdtxt(curText);
    cmdtxt.replace(0, prefix.length(), "");
    if (!rapidFireEnabled) {
        cmdtxt = cmdtxt.toLower();
    }

    if (cmdActive) {
        if (rapidFireEnabled) {
            /*
            \todo sort Qt::Return
            if (rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return)
            {
                emit appendHistory(curText, prefix.length());
                emit runCommand(curCmd, "RAPID_ENTER");
                curText.clear();
                clear();
                return;
            }
            else if (rapidChar == Qt::Key_Space)
            {
                updateCurrentText(curText + " ");
                emit runCommand(curCmd, cmdtxt + " ");
                return;
            }
            else
            {
                emit runCommand(curCmd, cmdtxt);
                return;
            }
            */
        }
        else {
            emit appendHistory(curText, prefix.length());
            emit runCommand(curCmd, cmdtxt);
        }
    }
    else {
        char cmd[MAX_STRING_LENGTH];
        strcpy(cmd, cmdtxt.toStdString().c_str());
        QString output = QString::fromStdString(actuator(cmd));
        emit appendHistory(curText + output, prefix.length());
    }

    if (!rapidFireEnabled) {
        clear();
    }
}

/* CheckSelection */
void
CmdPromptInput::checkSelection()
{
    debug_message("CmdPromptInput::checkSelection");
    if (this->hasSelectedText()) {
        this->deselect();
    }
}

/* checkCursorPosition */
void
CmdPromptInput::checkCursorPosition(int oldpos, int newpos)
{
    qDebug("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    if (this->hasSelectedText())
        this->deselect();
    if (newpos < prefix.length())
        this->setCursorPosition(prefix.length());
}

/* changeFormatting */
void
CmdPromptInput::changeFormatting(std::vector<QTextLayout::FormatRange> formats)
{
    QList<QInputMethodEvent::Attribute> attributes;
    for (int i=0; i<(int)(formats.size()); i++) {
        QTextLayout::FormatRange range = formats[i];
        QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        int start = range.start - this->cursorPosition();
        int length = range.length;
        QVariant value = range.format;
        attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
    }
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(this, &event);
}

/* clearFormatting */
void
CmdPromptInput::clearFormatting()
{
    std::vector<QTextLayout::FormatRange> formatting;
    changeFormatting(formatting);
}

/* applyFormatting */
void CmdPromptInput::applyFormatting()
{
    int prefixLength = prefix.length();

    int start = -1;
    int stop = -1;

    std::vector<QTextLayout::FormatRange> formats;

    //Bold Prefix
    QTextCharFormat formatPrefix;
    formatPrefix.setFontWeight(QFont::Bold);
    QTextLayout::FormatRange rangePrefix;
    rangePrefix.start = 0;
    rangePrefix.length = prefixLength;
    rangePrefix.format = formatPrefix;
    formats.push_back(rangePrefix);

    //Keywords
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop) {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor::fromRgb(0x00, 0x95, 0xFF));

        int rangeStart = -1;
        int rangeStop = -1;
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == ']') {
                rangeStop = i;
            }
            if (prefix.at(i) == '[' || prefix.at(i) == '/') {
                rangeStart = i;

                QTextLayout::FormatRange rangeKeyword;
                rangeKeyword.start = rangeStart+1;
                rangeKeyword.length = rangeStop-rangeStart-1;
                rangeKeyword.format = formatKeyword;
                formats.push_back(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    //Default Values
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if (start != -1 && stop != -1 && start < stop) {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor("#00AA00"));

        int rangeStart = -1;
        int rangeStop = -1;
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == '}') {
                rangeStop = i;
            }
            if (prefix.at(i) == '{') {
                rangeStart = i;

                QTextLayout::FormatRange rangeKeyword;
                rangeKeyword.start = rangeStart+1;
                rangeKeyword.length = rangeStop-rangeStart-1;
                rangeKeyword.format = formatKeyword;
                formats.push_back(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    changeFormatting(formats);
}

/* Update current text to "txt". */
void
CmdPromptInput::updateCurrentText(QString txt)
{
    int cursorPos = cursorPosition();
    if (!txt.startsWith(prefix)) {
        if (txt.length() < prefix.length())
            this->setText(prefix);
        else if (txt.length() != prefix.length())
            this->setText(prefix + txt);
        else
            this->setText(curText);
    }
    else
    {
        // input is okay so update curText
        curText = txt;
        this->setText(curText);
    }
    setCursorPosition(cursorPos);

    applyFormatting();
}

/* CmdPromptInput::checkEditedText txt
 */
void
CmdPromptInput::checkEditedText(QString txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        processInput();
    }
}

/* CheckChangedText. */
void
CmdPromptInput::checkChangedText(QString  txt)
{
    updateCurrentText(txt);
}

/* ContextMenuEvent. */
void
CmdPromptInput::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    QAction* copyAction = new QAction("&Copy", this);
    copyAction->setStatusTip("Copies the command prompt text to the clipboard.");
    connect(
        copyAction,
        &QAction::triggered,
        this,
        [=](void) {
            qApp->clipboard()->setText(curText.remove(0, prefix.length()));
        }
    );
    menu.addAction(copyAction);

    QAction* pasteAction = new QAction("&Paste", this);
    pasteAction->setStatusTip("Inserts the clipboard's text into the command prompt at the cursor position.");
    connect(pasteAction, &QAction::triggered, this, [=](void) { paste(); });
    menu.addAction(pasteAction);

    menu.addSeparator();

    QAction* settingsAction = new QAction("&Settings...", this);
    settingsAction->setStatusTip("Opens settings for the command prompt.");
    connect(settingsAction, &QAction::triggered, this, [=](void) { showSettings(); });
    menu.addAction(settingsAction);

    menu.exec(event->globalPos());
}

/* EventFilter */
bool
CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        if (isBlinking) {
            emit stopBlinking();
        }

        QKeyEvent* pressedKey = (QKeyEvent*)event;

        // NOTE: These shortcuts need to be caught since QLineEdit uses them
        if (pressedKey->matches(QKeySequence::Cut)) {
            pressedKey->accept();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Copy)) {
            pressedKey->accept();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Paste)) {
            pressedKey->accept();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::SelectAll)) {
            pressedKey->accept();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Undo)) {
            pressedKey->accept();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Redo)) {
            pressedKey->accept();
            return true;
        }

        int key = pressedKey->key();
        switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                pressedKey->accept();
                processInput();
                return true;
            case Qt::Key_Delete:
                pressedKey->accept();
                del();
                return true;
            case Qt::Key_Tab:
                pressedKey->accept();
                return true;
            case Qt::Key_Escape:
                pressedKey->accept();
                prefix = defaultPrefix;
                clear();
                emit appendHistory(curText + tr("*Cancel*"), prefix.length());
                return true;
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_F1:
            case Qt::Key_F2:
            case Qt::Key_F3:
            case Qt::Key_F4:
            case Qt::Key_F5:
            case Qt::Key_F6:
            case Qt::Key_F7:
            case Qt::Key_F8:
            case Qt::Key_F9:
            case Qt::Key_F10:
            case Qt::Key_F11:
            case Qt::Key_F12:
                pressedKey->accept();
                return true;
            case Qt::Key_Shift:
                pressedKey->ignore(); //we don't want to eat it, we just want to keep track of it
                emit prompt->shiftPressed();
                break;
            default:
                pressedKey->ignore();
                break;
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        switch (key) {
            case Qt::Key_Shift:
                releasedKey->ignore(); //we don't want to eat it, we just want to keep track of it
                emit prompt->shiftReleased();
                break;
            default:
                releasedKey->ignore();
        }
    }
    return QObject::eventFilter(obj, event);
}

/* . */
QLabel *
make_label(char label_str[100], QWidget *parent)
{
    return new QLabel(label_str, parent);
}

/* . */
QLabel *
make_label(double x, QWidget *parent)
{
    return new QLabel(QString::number(x), parent);
}

/* . */
QLabel *
make_label(uint32_t x, QWidget *parent)
{
    return new QLabel(QString::number(x), parent);
}

/* . */
QLabel *
make_translated_label(char *label_str, QWidget *parent)
{
    return new QLabel(translate_str(label_str), parent);
}

/* Create a details dialog object. */
EmbDetailsDialog::EmbDetailsDialog(QGraphicsScene* theScene, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    getInfo();
    mainWidget = createMainWidget();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(mainWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Embroidery Design Details"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* Destroy the details dialog. */
EmbDetailsDialog::~EmbDetailsDialog()
{
    QApplication::restoreOverrideCursor();
}

/* Create main widget. */
QWidget*
EmbDetailsDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);
    QGroupBox* groupBoxMisc = new QGroupBox(tr("General Information"), widget);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    for (int i=0; i<12; i++) {
        gridLayoutMisc->addWidget(make_translated_label((char*)details_labels[i], widget), i, 0, Qt::AlignLeft);
    }
    gridLayoutMisc->addWidget(make_label(stitchesTotal, widget), 0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesReal, widget), 1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesJump, widget), 2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesTrim, widget), 3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(colorTotal, widget), 4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(colorChanges, widget), 5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.left(), widget), 6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.top(), widget), 7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.right(), widget), 8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.bottom(), widget), 9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.width(), widget), 10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.height(), widget), 11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram

    /*
    QString str = "";
    for (int i = 0; i < num_bins; i++) {
        str += QString::number(binSize * (i), 'f', 1);
        str += " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: ";
        str += QString::number(bin[i]) + "\n\n";
    }

    QGridLayout* grid = new QGridLayout(_mainWin);
    grid->setSpacing(2);
    grid->addWidget(new QLabel(translate_str("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(translate_str("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;

    for (int i = 0; i < num_colors; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = pattern->thread_list->thread[i].color;
        //palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        qDebug("size: %d i: %d", stitchLengths.size(), i);
        //grid->addWidget(new QLabel(QString::number(stitch_lengths[i]) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

     */


    QDialogButtonBox buttonbox(Qt::Horizontal, this);
    QPushButton button(this);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), this, SLOT(accept()));

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    //vboxLayoutMain->addWidget(groupBoxDist);
    //vboxLayoutMain->addWidget(buttonbox);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    return scrollArea;
}

/* Get information from the embroidery.
 *
 * TODO: Move majority of this code into libembroidery.
 * TODO: generate a temporary pattern from the scene data.
 * TODO: grab this information from the pattern
 *
 * TODO: embStitchList_count(pattern->stitchList, TOTAL);
 */
void
EmbDetailsDialog::getInfo(void)
{
    debug_message("designDetails()");

    boundingRect.setRect(0, 0, 50, 100); //TODO: embPattern_calcBoundingBox(pattern);

    int bin[MAX_HISTOGRAM_BINS+1];

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern\n");
        return;
    }

    /* TODO: This is temporary. Generate actual pattern data from the scene. */
    /* TODO: This convenience function is messed up. */

    //boundingRect = embPattern_calcBoundingBox(pattern);

    EmbVector minimum, maximum;
    EmbStitch last_pos;
    int num_bins = 10;
    stitchesTotal = pattern->stitch_list->count;
    stitchesReal = 0;
    stitchesJump = 0;
    stitchesTrim = 0;
    colorTotal = pattern->thread_list->count;
    colorChanges  = 0; //TODO: embThreadList_count(pattern->threadList, CHANGES);

    int unknown_stitches = 0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;
    double length = 0.0;

    minimum.x = 1000.0f;
    minimum.y = 1000.0f;
    maximum.x = 0.0f;
    maximum.y = 0.0f;

    if (pattern->stitch_list->count == 0) {
        QMessageBox::warning(_mainWin,
            translate_str("No Design Loaded"),
            translate_str("<b>A design needs to be loaded or created before details can be determined.</b>"));
        embPattern_free(pattern);
        return;
    }

    QVector<double> stitchLengths;
    double totalColorLength = 0.0;
    last_pos = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - last_pos.x;
        delta.y = st.y - last_pos.y;
        length = embVector_length(delta);
        totalColorLength += length;
        // can't count first normal stitch
        if (last_pos.flags != NORMAL) {
            length = 0.0;
        }
        if (!(st.flags & (JUMP | TRIM))) {
            stitchesReal++;
            if (length > max_stitchlength) {
                max_stitchlength = length;
                number_of_maxlength_stitches = 0;
            }
            if (length == max_stitchlength) {
                number_of_maxlength_stitches++;
            }
            if (length > 0 && length < min_stitchlength) {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if (length == min_stitchlength) {
                number_of_minlength_stitches++;
            }
            total_stitchlength += length;
            if (last_pos.x < minimum.x) {
                minimum.x = last_pos.x;
            }
            if (last_pos.y < minimum.y) {
                minimum.y = last_pos.y;
            }
            if (last_pos.x > maximum.x) {
                maximum.x = last_pos.x;
            }
            if (last_pos.y > maximum.y) {
                maximum.y = last_pos.y;
            }
        }
        if (st.flags & JUMP) {
            stitchesJump++;
        }
        if (st.flags & TRIM) {
            stitchesTrim++;
        }
        if (st.flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colorChanges++;
        }
        if (st.flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
        if (st.flags | (JUMP & TRIM & STOP & END)) {
            unknown_stitches++;
        }
        last_pos = st;
    }

    /* Second pass to fill bins now that we know max stitch length. */
    double binSize = max_stitchlength / num_bins;

    for (int i = 0; i <= num_bins; i++) {
        bin[i]=0;
    }

    last_pos = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (last_pos.flags == NORMAL && st.flags == NORMAL) {
            EmbVector delta;
            delta.x = st.x - last_pos.x;
            delta.y = st.y - last_pos.y;
            length = embVector_length(delta);
            bin[int(floor(num_bins*length/max_stitchlength))]++;
        }
        last_pos = st;
    }
}

/* . */
void
MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    _mainWin->openFile();
}

/* . */
void
MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if (useColor) {
        painter.fillRect(rect, bgColor);
    }
    else {
        painter.fillRect(rect, background());
    }

    //Then overlay the texture
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if (useLogo) {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

/* Zoom extents all subwindows. */
void MdiArea::zoomExtentsAllSubWindows()
{
    foreach(QMdiSubWindow* window, subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            if (mdiWin->gview) {
                mdiWin->gview->recalculateLimits();
                mdiWin->gview->zoomExtents();
            }
        }
    }
}

/* Force repaint. */
void MdiArea::forceRepaint()
{
    //HACK: Take that QMdiArea!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

/* Construct a new MdiWindow object.
 *
 * theIndex, parent, wflags
 *
 * WARNING:
 *   DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
 *   AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
 *   ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
 */
MdiWindow::MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    QString aName;
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    this->setWindowTitle(curFile);

    this->setWindowIcon(_mainWin->create_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new View(gscene, this);

    setWidget(gview);

    gview->setFocusProxy(prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    prompt->setHistory(promptHistory);
    promptInputList.push_back("");
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; //TODO: color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    showMaximized();

    setFocusPolicy(Qt::WheelFocus);
    setFocus();

    onWindowActivated();
}

/* Destroy the MdiWindow object. */
MdiWindow::~MdiWindow()
{
    debug_message("MdiWindow Destructor()");
}

/* Save file to "fileName". */
bool
MdiWindow::saveFile(std::string fileName)
{
    return save_current_file(fileName);
}

/* Load file to this subwindow. */
bool
MdiWindow::loadFile(std::string fileName)
{
    debug_message("MdiWindow loadFile()");

    QRgb tmpColor = curColor;

    QFile file(QString::fromStdString(fileName));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QString error = translate_str("Cannot read file")
            + QString::fromStdString(fileName) + ": " + file.errorString();
        QMessageBox::warning(this, tr("Error reading file"), error);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ext = fileExtension(fileName);
    qDebug("ext: %s", qPrintable(ext));

    // Read
    EmbPattern* p = embPattern_create();
    if (!p) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }
    int readSuccessful = 0;
    std::string readError;
    int reader = emb_identify_format(fileName.c_str());
    if (reader < 0) {
        readSuccessful = 0;
        readError = "Unsupported read file type: " + fileName;
        debug_message("Unsupported read file type: " + fileName);
    }
    else {
        readSuccessful = embPattern_readAuto(p, fileName.c_str());
        if (!readSuccessful) {
            readError = "Reading file was unsuccessful: " + fileName;
            debug_message("Reading file was unsuccessful: " + fileName);
        }
    }

    if (readSuccessful) {
        debug_message("Starting to load the read file.");
        //embPattern_moveStitchListToPolylines(p); //TODO: Test more

        int stitchCount = p->stitch_list->count;
        qDebug("%d", stitchCount);
        for (int i=0; i<stitchCount; i++) {
            EmbStitch st = p->stitch_list->stitch[i];
            // int color = 0;
            QPainterPath stitchPath;
            bool firstPoint = true;
            EmbVector pos = {0, 0};
            for (; (st.flags <= 2) && (i<stitchCount); i++) {
                st = p->stitch_list->stitch[i];
                // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                pos.x = st.x;
                pos.y = -st.y;
                qDebug("%d %f %f", i, pos.x, pos.y);

                /* BUG: This means that the trim information is forgot in loading to a
                 * polyline. We should have a custom class for this.
                 */
                if (firstPoint || (st.flags == JUMP) || (st.flags == TRIM)) {
                    stitchPath.moveTo(pos.x, pos.y);
                }
                else {
                    if (st.flags == NORMAL) {
                        stitchPath.lineTo(pos.x, pos.y);
                    }
                    firstPoint = false;
                }
            }

            add_polyline(stitchPath, "OBJ_RUBBER_OFF");
        }

        for (int i=0; i<p->geometry->count; i++) {
            char command_str[MAX_STRING_LENGTH];
            EmbGeometry g = p->geometry->geometry[i];
            curColor = qRgb(g.color.r, g.color.g, g.color.b);
            if (g.type == EMB_CIRCLE) {
                EmbCircle c = g.object.circle;
                sprintf(command_str, "add circle %f %f %f %i %s",
                    c.center.x, c.center.y, c.radius, false, "OBJ_RUBBER_OFF");
                // NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                actuator(command_str); //TODO: fill
            }
            if (g.type == EMB_ELLIPSE) {
                EmbEllipse e = g.object.ellipse;
                sprintf(command_str, "add ellipse %f %f %f %f %i %i %s",    e.center.x, e.center.y, embEllipse_width(e),
                    embEllipse_height(e), 0, false, "OBJ_RUBBER_OFF");
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                actuator(command_str); //TODO: rotation and fill
            }
            if (g.type == EMB_LINE) {
                EmbLine li = g.object.line;
                sprintf(command_str,
                    "add line %f %f %f %f %i %s",
                    li.start.x,
                    li.start.y,
                    li.end.x,
                    li.end.y,
                    0,
                    "OBJ_RUBBER_OFF");
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                actuator(command_str); //TODO: rotation
            }
            if (g.type == EMB_PATH) {
                // TODO: This is unfinished. It needs more work
                QPainterPath pathPath;
                EmbArray* curPointList = g.object.path.pointList;
                EmbPoint pp = curPointList->geometry[0].object.point;
                // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                pathPath.moveTo(pp.position.x, -pp.position.y);
                for (int j=1; j<curPointList->count; j++) {
                    EmbPoint pp = curPointList->geometry[j].object.point;
                    pathPath.lineTo(pp.position.x, -pp.position.y);
                }

                QPen loadPen(qRgb(g.color.r, g.color.g, g.color.b));
                loadPen.setWidthF(0.35);
                loadPen.setCapStyle(Qt::RoundCap);
                loadPen.setJoinStyle(Qt::RoundJoin);

                /*
                EmbVector v = {0.0, 0.0};
                Geometry* obj = new Geometry(v, pathPath, loadPen.color().rgb(), Qt::SolidLine);
                obj->objRubberMode = "OBJ_RUBBER_OFF";
                gscene->addItem(obj);
                */
            }
            if (p->geometry->geometry[i].type == EMB_POINT) {
                EmbPoint po = g.object.point;
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                sprintf(command_str,
                    "add point %f %f",
                    po.position.x,
                    po.position.y);
                actuator(command_str);
            }
            if (p->geometry->geometry[i].type == EMB_POLYGON) {
                EmbPolygon polygon = g.object.polygon;
                QPainterPath polygonPath;
                bool firstPoint = false;
                EmbReal startX = 0, startY = 0;
                EmbReal x = 0, y = 0;
                EmbArray* curPointList = polygon.pointList;
                for (int j=0; j<curPointList->count; j++) {
                    EmbPoint pp = curPointList->geometry[j].object.point;
                    x = pp.position.x;
                    y = -pp.position.y;
                    // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    if (firstPoint) {
                        polygonPath.lineTo(x,y);
                    }
                    else {
                        polygonPath.moveTo(x,y);
                        firstPoint = true;
                        startX = x;
                        startY = y;
                    }
                }

                polygonPath.translate(-startX, -startY);
                //AddPolygon(startX, startY, polygonPath, "OBJ_RUBBER_OFF");
            }
            /* NOTE: Polylines should only contain NORMAL stitches. */
            if (p->geometry->geometry[i].type == EMB_POLYLINE) {
                EmbPolygon polyline = g.object.polyline;
                QPainterPath polylinePath;
                bool firstPoint = false;
                EmbVector start = {0, 0};
                EmbVector pos = {0, 0};
                EmbArray* curPointList = polyline.pointList;
                for (int j=0; j<curPointList->count; j++) {
                    EmbPoint pp = curPointList->geometry[j].object.point;
                    // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                    pos.x = pp.position.x;
                    pos.y = -pp.position.y;

                    if (firstPoint) {
                        polylinePath.lineTo(pos.x, pos.y);
                    }
                    else {
                        polylinePath.moveTo(pos.x, pos.y);
                        firstPoint = true;
                        start = pos;
                    }
                }

                polylinePath.translate(-start.x, -start.y);
                //AddPolyline(start.x, start.y, polylinePath, "OBJ_RUBBER_OFF");
            }
            if (g.type == EMB_RECT) {
                EmbRect r = g.object.rect;
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                sprintf(command_str,
                      "add rectangle %f %f %f %f %i %i %s",
                    r.left,
                    r.top,
                    r.right - r.left,
                    r.bottom - r.top,
                    0,
                    false,
                    "OBJ_RUBBER_OFF");
                actuator(command_str);
            }
        }

        setCurrentFile(QString::fromStdString(fileName));
        statusbar->showMessage("File loaded.");
        QString stitches;
        stitches.setNum(stitchCount);

        if (settings[ST_GRID_LOAD_FROM_FILE].i) {
            //TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
        }

        QApplication::restoreOverrideCursor();
    }
    else {
        QMessageBox::warning(this, translate_str("Error reading pattern"), translate_str(readError.c_str()));
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, translate_str("Error reading pattern"), translate_str("Cannot read pattern"));
    }
    embPattern_free(p);

    // Clear the undo stack so it is not possible to undo past this point.
    gview->undoStack->clear();

    curColor = tmpColor;

    fileWasLoaded = true;
    _mainWin->setUndoCleanIcon(fileWasLoaded);
    return fileWasLoaded;
}

/* Print this subwindow. */
void MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (settings[ST_PRINT_DISABLE_BG].i) {
            //Save current bg
            QBrush brush = gview->backgroundBrush();
            //Save ink by not printing the bg at all
            gview->setBackgroundBrush(Qt::NoBrush);
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
            //Restore the bg
            gview->setBackgroundBrush(brush);
        }
        else {
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
        }
    }
}


/* Save BMC
 *
 * TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?
 *
 * TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
 * TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
 * NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
 */
void MdiWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    // TODO: Make BMC background into it's own setting?
    if (settings[ST_PRINT_DISABLE_BG].i) {
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

/* Set current file. */
void
MdiWindow::setCurrentFile(QString fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

/* Get short current file. */
QString
MdiWindow::getShortCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

/* Get file extension from fileName. */
QString
fileExtension(std::string fileName)
{
    return QFileInfo(QString::fromStdString(fileName)).suffix().toLower();
}

/* Close event. */
void
MdiWindow::closeEvent(QCloseEvent* /*e*/)
{
    debug_message("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

/* On window activated. */
void
MdiWindow::onWindowActivated()
{
    debug_message("MdiWindow onWindowActivated()");
    gview->undoStack->setActive(true);
    _mainWin->setUndoCleanIcon(fileWasLoaded);
    statusbar->buttons["SNAP"]->setChecked(gscene->property("ENABLE_SNAP").toBool());
    statusbar->buttons["GRID"]->setChecked(gscene->property("ENABLE_GRID").toBool());
    statusbar->buttons["RULER"]->setChecked(gscene->property("ENABLE_RULER").toBool());
    statusbar->buttons["ORTHO"]->setChecked(gscene->property("ENABLE_ORTHO").toBool());
    statusbar->buttons["POLAR"]->setChecked(gscene->property("ENABLE_POLAR").toBool());
    statusbar->buttons["QSNAP"]->setChecked(gscene->property("ENABLE_QSNAP").toBool());
    statusbar->buttons["QTRACK"]->setChecked(gscene->property("ENABLE_QTRACK").toBool());
    statusbar->buttons["LWT"]->setChecked(gscene->property("ENABLE_LWT").toBool());
    prompt->setHistory(promptHistory);
}

/* SizeHint */
QSize
MdiWindow::sizeHint()
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

/* promptInputPrevNext. */
void
MdiWindow::promptInputPrevNext(bool prev)
{
    if (promptInputList.size() == 0) {
        if (prev) {
            QMessageBox::critical(this,
                tr("Prompt Previous Error"),
                tr("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            QMessageBox::critical(this,
                tr("Prompt Next Error"),
                tr("The prompt input is empty! Please report this as a bug!"));
        }
        debug_message("The prompt input is empty! Please report this as a bug!");
    }
    else {
        if (prev) {
            promptInputNum--;
        }
        else {
            promptInputNum++;
        }
        int maxNum = promptInputList.size();
        if (promptInputNum < 0) {
            promptInputNum = 0;
            prompt->setCurrentText("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            prompt->setCurrentText("");
        }
        else {
            prompt->setCurrentText(promptInputList.at(promptInputNum));
        }
    }
}

/* Create a property editor object. */
PropertyEditor::PropertyEditor(QString  iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    qDebug("Creating PropertyEditor...");
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; // \todo Make customizable
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0; // \todo Load this from settings and provide function for updating from settings
    precisionLength = 4; // \todo Load this from settings and provide function for updating from settings

    signalMapper = new QSignalMapper(this);

    fieldOldText = "";
    fieldNewText = "";
    fieldVariesText = "*Varies*";
    fieldYesText = "Yes";
    fieldNoText = "No";
    fieldOnText = "On";
    fieldOffText = "Off";

    for (int i=0; i<GB_TOTAL; i++) {
        groupBoxes[i] = createGroupBox(group_box_data[2*i+0], group_box_data[2*i+1]);
    }

    QWidget* widgetMain = new QWidget(this);

    QWidget* widgetSelection = new QWidget(this);
    QHBoxLayout* hboxLayoutSelection = new QHBoxLayout(this);
    hboxLayoutSelection->addWidget(createComboBoxSelected());
    hboxLayoutSelection->addWidget(createToolButtonQSelect());
    hboxLayoutSelection->addWidget(createToolButtonPickAdd());
    widgetSelection->setLayout(hboxLayoutSelection);

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    for (int i=0; i<GB_TOTAL; i++) {
        vboxLayoutProperties->addWidget(groupBoxes[i]);
    }
    vboxLayoutProperties->addStretch(1);
    widgetProperties->setLayout(vboxLayoutProperties);
    scrollProperties->setWidget(widgetProperties);
    scrollProperties->setWidgetResizable(true);

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(widgetSelection);
    vboxLayoutMain->addWidget(scrollProperties);
    widgetMain->setLayout(vboxLayoutMain);

    setWidget(widgetMain);
    setWindowTitle(translate_str("Properties"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    hideAllGroups();

    connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)));

    focusWidget = widgetToFocus;
    this->installEventFilter(this);
}

/* Destroy the property editor. */
PropertyEditor::~PropertyEditor()
{
}

/* Event filter. */
bool
PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key) {
            case Qt::Key_Escape:
                if (focusWidget)
                    focusWidget->setFocus(Qt::OtherFocusReason);
                return true;
                break;
            default:
                pressedKey->ignore();
        }
    }
    return QObject::eventFilter(obj, event);
}

/**
 * \todo document this
 */
QComboBox*
PropertyEditor::createComboBoxSelected()
{
    comboBoxSelected = new QComboBox(this);
    comboBoxSelected->addItem(translate_str("No Selection"));
    return comboBoxSelected;
}

/**
 * \todo document this
 */
QToolButton*
PropertyEditor::createToolButtonQSelect()
{
    toolButtonQSelect = new QToolButton(this);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); // \todo Better Description
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

/**
 *
 */
QToolButton*
PropertyEditor::createToolButtonPickAdd()
{
    // \todo Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = new QToolButton(this);
    updatePickAddModeButton(pickAdd);
    connect(toolButtonPickAdd, SIGNAL(clicked(bool)), this, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

/**
 *
 */
void
PropertyEditor::updatePickAddModeButton(bool pickAddMode)
{
    pickAdd = pickAddMode;
    if (pickAdd) {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "pickadd" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/" + "picknew" + ".png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickNew");
        toolButtonPickAdd->setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

/* . */
void
PropertyEditor::togglePickAddMode()
{
    emit pickAddModeToggled();
}

/* . */
void
PropertyEditor::setSelectedItems(std::vector<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.empty()) {
        comboBoxSelected->addItem(translate_str("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int numPerType[50];
    for (int i=0; i<50; i++) {
        numPerType[i] = 0;
    }

    foreach(QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        int objType = item->type();
        typeSet.insert(objType);

        if ((objType > OBJ_TYPE_ARC) && (objType < OBJ_TYPE_UNKNOWN)) {
            numPerType[objType - OBJ_TYPE_BASE]++;
        }
        else {
            numPerType[OBJ_TYPE_UNKNOWN - OBJ_TYPE_BASE]++;
        }
    }

    int numTypes = typeSet.size();

    /* Populate the selection comboBox. */
    if (numTypes > 1) {
        comboBoxSelected->addItem(translate_str("Varies") + " (" + QString().setNum(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

    QString comboBoxStr;
    foreach(int objType, typeSet) {
        QString num = QString().setNum(numPerType[objType - OBJ_TYPE_BASE]);
        if ((objType > OBJ_TYPE_ARC) && (objType < OBJ_TYPE_UNKNOWN)) {
            comboBoxStr = tr(object_names[objType - OBJ_TYPE_BASE]) + "(" + num + ")";
        }
        else {
            comboBoxStr = translate_str("Unknown") + " (" + QString().setNum(numPerType[objType - OBJ_TYPE_UNKNOWN]) + ")";
        }

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    /* Load Data into the fields. */

    //Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    foreach(QGraphicsItem* item, itemList) {
        if (!item) continue;

        // \todo load data into the General field

        int objType = item->type();
        Geometry* obj = static_cast<Geometry*>(item);
        if (!obj) {
            continue;
        }
        if (objType == OBJ_TYPE_ARC) {
            updateLineEditNumIfVaries(lineEdits["arc-center-x"], obj->scenePos().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc-center-y"], -obj->scenePos().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_radius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(lineEdits["arc_start_angle"], obj->objectStartAngle(), true);
            updateLineEditNumIfVaries(lineEdits["arc_end_angle"], obj->objectEndAngle(), true);
            updateLineEditNumIfVaries(lineEdits["arc_start_x"], obj->objectStartPoint().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc_start_y"], -obj->objectStartPoint().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_end_x"], obj->objectEndPoint().x(), false);
            updateLineEditNumIfVaries(lineEdits["arc_end_y"], -obj->objectEndPoint().y(), false);
            updateLineEditNumIfVaries(lineEdits["arc_area"], obj->objectArea(), false);
            updateLineEditNumIfVaries(lineEdits["arc_length"], obj->objectArcLength(), false);
            updateLineEditNumIfVaries(lineEdits["arc_chord"], obj->objectChord(), false);
            updateLineEditNumIfVaries(lineEdits["arc_inc_angle"], obj->objectIncludedAngle(), true);
            updateComboBoxBoolIfVaries(comboBoxes["arc-clockwise"], obj->objectClockwise(), true);
        }
        else if (objType == OBJ_TYPE_BLOCK) {
            // \todo load block data
        }
        else if (objType == OBJ_TYPE_CIRCLE) {
            updateLineEditNumIfVaries(lineEdits["circle_center_x"], obj->scenePos().x(), false);
            updateLineEditNumIfVaries(lineEdits["circle_center_y"], -obj->scenePos().y(), false);
            updateLineEditNumIfVaries(lineEdits["circle_radius"], obj->objectRadius(), false);
            updateLineEditNumIfVaries(lineEdits["circle_diameter"], obj->objectDiameter(), false);
            updateLineEditNumIfVaries(lineEdits["circle_area"], obj->objectArea(), false);
            updateLineEditNumIfVaries(lineEdits["circle_circumference"], obj->objectCircumference(), false);
        }
        else if (objType == OBJ_TYPE_DIMALIGNED) {
            // \todo load aligned dimension data
        }
        else if (objType == OBJ_TYPE_DIMANGULAR) {
            // \todo load angular dimension data
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH) {
            // \todo load arclength dimension data
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER) {
            // \todo load diameter dimension data
        }
        else if (objType == OBJ_TYPE_DIMLEADER) {
            // \todo load leader dimension data
        }
        else if (objType == OBJ_TYPE_DIMLINEAR) {
            // \todo load linear dimension data
        }
        else if (objType == OBJ_TYPE_DIMORDINATE) {
            // \todo load ordinate dimension data
        }
        else if (objType == OBJ_TYPE_DIMRADIUS) {
            // \todo load radius dimension data
        }
        else if (objType == OBJ_TYPE_ELLIPSE) {
            updateLineEditNumIfVaries(lineEdits["ellipse-center-x"], obj->scenePos().x(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-center-y"], -obj->scenePos().y(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-radius-major"], obj->objectRadiusMajor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-radius-minor"], obj->objectRadiusMinor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-diameter-major"], obj->objectDiameterMajor(), false);
            updateLineEditNumIfVaries(lineEdits["ellipse-diameter-minor"], obj->objectDiameterMinor(), false);
        }
        else if (objType == OBJ_TYPE_IMAGE) {
            // \todo load image data
        }
        else if (objType == OBJ_TYPE_INFINITELINE) {
            // \todo load infinite line data
        }
        else if (objType == OBJ_TYPE_LINE) {
            updateLineEditNumIfVaries(lineEdits["line-start-x"], obj->objectEndPoint1().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-start-y"], -obj->objectEndPoint1().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-end-x"], obj->objectEndPoint2().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-end-y"], -obj->objectEndPoint2().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-delta-x"], obj->objectDelta().x(), false);
            updateLineEditNumIfVaries(lineEdits["line-delta-y"], -obj->objectDelta().y(), false);
            updateLineEditNumIfVaries(lineEdits["line-angle"], obj->objectAngle(), true);
            updateLineEditNumIfVaries(lineEdits["line-length"], obj->objectLength(), false);
        }
        else if (objType == OBJ_TYPE_PATH) {
            // \todo load path data
        }
        else if (objType == OBJ_TYPE_POINT) {
            updateLineEditNumIfVaries(lineEdits["point-x"],  obj->scenePos().x(), false);
            updateLineEditNumIfVaries(lineEdits["point-y"], -obj->scenePos().y(), false);
        }
        else if (objType == OBJ_TYPE_POLYGON) {
            // \todo load polygon data
        }
        else if (objType == OBJ_TYPE_POLYLINE) {
            // \todo load polyline data
        }
        else if (objType == OBJ_TYPE_RAY) {
            // \todo load ray data
        }
        else if (objType == OBJ_TYPE_RECTANGLE) {
            QPointF corn1 = obj->objectTopLeft();
            QPointF corn2 = obj->objectTopRight();
            QPointF corn3 = obj->objectBottomLeft();
            QPointF corn4 = obj->objectBottomRight();

            updateLineEditNumIfVaries(lineEdits["rectangle_corner1_x"], corn1.x(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner1_y"], -corn1.y(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner2_x"], corn2.x(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner2_y"], -corn2.y(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner3_x"], corn3.x(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner3_y"], -corn3.y(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner4_x"], corn4.x(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_corner4_y"], -corn4.y(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_width"], obj->objectWidth(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_height"], -obj->objectHeight(), false);
            updateLineEditNumIfVaries(lineEdits["rectangle_area"], obj->objectArea(), false);
        }
        else if (objType == OBJ_TYPE_TEXTMULTI) {
            // \todo load multiline text data
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE) {
            updateLineEditStrIfVaries(lineEdits["text-single-contents"], obj->objText);
            updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->objTextFont);
            updateComboBoxStrIfVaries(comboBoxes["text-single-justify"], obj->objTextJustify, {});
            updateLineEditNumIfVaries(lineEdits["text-single-height"], obj->text_size, false);
            updateLineEditNumIfVaries(lineEdits["text-single-rotation"], -obj->rotation(), true);
            updateLineEditNumIfVaries(lineEdits["text-single-x"], obj->scenePos().x(), false);
            updateLineEditNumIfVaries(lineEdits["text-single-y"], -obj->scenePos().y(), false);
            updateComboBoxBoolIfVaries(comboBoxes["text-single-backward"], obj->flags & PROP_BACKWARD, true);
            updateComboBoxBoolIfVaries(comboBoxes["text-single-upside-down"], obj->flags & PROP_UPSIDEDOWN, true);
        }
    }

    /* Only show fields if all objects are the same type. */
    if (numTypes == 1) {
        foreach(int objType, typeSet) {
            showGroups(objType);
        }
    }
}

/* . */
void
PropertyEditor::updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        lineEdit->setText(fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        lineEdit->setText(fieldVariesText);
    }
}

void
PropertyEditor::updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) precision = precisionAngle;
    else                  precision = precisionLength;

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    QString negativeZero = "-0.";
    for (int i = 0; i < precision; ++i)
        negativeZero.append('0');
    if (fieldNewText == negativeZero)
        fieldNewText = negativeZero.replace("-", "");

    if (fieldOldText.isEmpty())
        lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText)
        lineEdit->setText(fieldVariesText);
}

/**
 *
 */
void
PropertyEditor::updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString  str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //qDebug("old: %d %s, new: %d %s", oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        if (fontComboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            fontComboBox->addItem(fieldVariesText);
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

/* . */
void
PropertyEditor::updateComboBoxStrIfVaries(QComboBox* comboBox, QString str, std::vector<std::string> strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        int n = string_array_length(justify_options);
        for (int i=0; i<n; i++) {
            QString s(justify_options[i]);
            comboBox->addItem(s, s);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/* . */
void PropertyEditor::updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if (yesOrNoText) {
        if (val) {
            fieldNewText = fieldYesText;
        }
        else {
            fieldNewText = fieldNoText;
        }
    }
    else {
        if (val) fieldNewText = fieldOnText;
        else    fieldNewText = fieldOffText;
    }

    if (fieldOldText.isEmpty()) {
        if (yesOrNoText) {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/* . */
void
PropertyEditor::showGroups(int objType)
{
    for (int i=0; i<GB_TOTAL; i++) {
        if (group_box_ids[i]== objType) {
            groupBoxes[i]->show();
        }
    }
}

/* . */
void
PropertyEditor::showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* NOTE: General group will never be hidden. */
void
PropertyEditor::hideAllGroups(void)
{
    for (int i=0; i<GB_TOTAL; i++) {
        if (strcmp(group_box_types[i], "general")) {
            groupBoxes[i]->hide();
        }
    }
}

/*
 * TODO: DimAligned, DimAngular, DimArcLength, DimDiameter,
 *  DimLeader, DimLinear, DimOrdinate, DimRadius
 */
void PropertyEditor::clearAllFields()
{
    for (int i=0; strcmp(all_line_editors[i].key, "END"); i++) {
        if (!strcmp(all_line_editors[i].type, "double")) {
            lineEdits[all_line_editors[i].key]->clear();
        }
        if (!strcmp(all_line_editors[i].type, "combobox")) {
            comboBoxes[all_line_editors[i].key]->clear();
        }
        if (!strcmp(all_line_editors[i].type, "fontcombobox")) {
            comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); //NOTE: Do not clear comboBoxTextSingleFont
            comboBoxTextSingleFont->setProperty("FontFamily", "");
        }
    }
}

/**
 * .
 */
QGroupBox *
PropertyEditor::createGroupBox(const char *group_box_key, const char *title)
{
    QGroupBox *gb = new QGroupBox(tr(title), this);
    gb->setObjectName(group_box_key);

    int group_box_type = OBJ_TYPE_BASE;
    int n_group_box_types = string_array_length(group_box_types);
    for (int i=0; i<n_group_box_types; i++) {
        if (!strcmp(group_box_types[i], group_box_key)) {
            group_box_type = group_box_ids[i];
            break;
        }
    }

    QFormLayout* formLayout = new QFormLayout(this);

    int n = (sizeof all_line_editors)/(sizeof all_line_editors[0]);
    for (int i=0; i<n-1; i++) {
        LineEditData gbd = all_line_editors[i];
        if (strcmp(all_line_editors[i].key, group_box_key)) {
            continue;
        }
        std::string key(gbd.key);
        toolButtons[key] = createToolButton(gbd.icon, tr(gbd.label));
        if (!strcmp(gbd.type, "double")) {
            if (strlen(gbd.map_signal) == 0) {
                lineEdits[key] = createLineEdit(gbd.type, false);
            }
            else {
                lineEdits[key] = createLineEdit(gbd.type, true);
                mapSignal(lineEdits[key], gbd.map_signal, group_box_type);
            }
            formLayout->addRow(toolButtons[key], lineEdits[key]);
        }
        if (!strcmp(gbd.type, "combobox")) {
            comboBoxes[key] = new QComboBox(this);
            if (strlen(gbd.map_signal) == 0) {
                comboBoxes[key]->setDisabled(true);
            }
            else {
                comboBoxes[key]->setDisabled(false);
            }
            formLayout->addRow(toolButtons[key], comboBoxes[key]);
        }
        if (!strcmp(gbd.type, "fontcombobox")) {
            comboBoxTextSingleFont = new QFontComboBox(this);
            comboBoxTextSingleFont->setDisabled(false);
            formLayout->addRow(toolButtons[key], comboBoxTextSingleFont);
        }
    }
    gb->setLayout(formLayout);
    return gb;
}

/**
 * .
 */
QToolButton*
PropertyEditor::createToolButton(QString iconName, QString txt)
{
    QToolButton* tb = new QToolButton(this);
    tb->setIcon(QIcon(iconDir + "/" + iconName + ".png"));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

/**
 * .
 */
QLineEdit*
PropertyEditor::createLineEdit(QString  validatorType, bool readOnly)
{
    QLineEdit* le = new QLineEdit(this);
    if (validatorType == "int") {
        le->setValidator(new QIntValidator(le));
    }
    else if (validatorType == "double") {
        le->setValidator(new QDoubleValidator(le));
    }
    le->setReadOnly(readOnly);
    return le;
}

/* PropertyEditor::mapSignal fieldObj name value
 */
void
PropertyEditor::mapSignal(QObject* fieldObj, QString  name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        connect(fieldObj, SIGNAL(activated(QString)), signalMapper, SLOT(map()));
    }

    signalMapper->setMapping(fieldObj, fieldObj);
}

/**
 * .
 */
void
PropertyEditor::fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) return;

    qDebug("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType)
            continue;

        Geometry* obj = static_cast<Geometry*>(item);
        if (!obj) {
            continue;
        }

        switch(objType) {
            case OBJ_TYPE_ARC:
                if (objName == "lineEditArcCenterX") {
                    obj->setX(lineEdits["arc-center-x"]->text().toDouble());
                }
                if (objName == "lineEditArcCenterY") {
                    obj->setY(-lineEdits["arc-center-y"]->text().toDouble());
                }
                if (objName == "lineEditArcRadius") {
                    obj->setObjectRadius(lineEdits["arc_radius"]->text().toDouble());
                }
                if (objName == "lineEditArcStartAngle") {
                    obj->setObjectStartAngle(lineEdits["arc_start_angle"]->text().toDouble());
                }
                if (objName == "lineEditArcEndAngle") {
                    obj->setObjectEndAngle(lineEdits["arc_end_angle"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_BLOCK: // \todo field editing
                break;
            case OBJ_TYPE_CIRCLE:
                if (objName == "lineEditCircleCenterX") {
                    obj->setX(lineEdits["circle_center_x"]->text().toDouble());
                }
                if (objName == "lineEditCircleCenterY") {
                    obj->setY(-lineEdits["circle_center_y"]->text().toDouble());
                }
                if (objName == "lineEditCircleRadius") {
                    obj->setObjectRadius(lineEdits["circle_radius"]->text().toDouble());
                }
                if (objName == "lineEditCircleDiameter") {
                    obj->setObjectDiameter(lineEdits["circle_diameter"]->text().toDouble());
                }
                if (objName == "lineEditCircleArea") {
                    obj->setObjectArea(lineEdits["circle_area"]->text().toDouble());
                }
                if (objName == "lineEditCircleCircumference") {
                    obj->setObjectCircumference(lineEdits["circle_circumference"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_DIMALIGNED: // \todo field editing
                break;
            case OBJ_TYPE_DIMANGULAR: // \todo field editing
                break;
            case OBJ_TYPE_DIMARCLENGTH: // \todo field editing
                break;
            case OBJ_TYPE_DIMDIAMETER: // \todo field editing
                break;
            case OBJ_TYPE_DIMLEADER: // \todo field editing
                break;
            case OBJ_TYPE_DIMLINEAR: // \todo field editing
                break;
            case OBJ_TYPE_DIMORDINATE: // \todo field editing
                break;
            case OBJ_TYPE_DIMRADIUS: // \todo field editing
                break;
            case OBJ_TYPE_ELLIPSE:
                if (objName == "lineEditEllipseCenterX") {
                    obj->setX(lineEdits["ellipse-center-x"]->text().toDouble());
                }
                if (objName == "lineEditEllipseCenterY") {
                    obj->setY(-lineEdits["ellipse-center-y"]->text().toDouble());
                }
                if (objName == "lineEditEllipseRadiusMajor") {
                    obj->setObjectRadiusMajor(lineEdits["ellipse-radius-major"]->text().toDouble());
                }
                if (objName == "lineEditEllipseRadiusMinor") {
                    obj->setObjectRadiusMinor(lineEdits["ellipse-radius-minor"]->text().toDouble());
                }
                if (objName == "lineEditEllipseDiameterMajor") {
                    obj->setObjectDiameterMajor(lineEdits["ellipse-diameter-major"]->text().toDouble());
                }
                if (objName == "lineEditEllipseDiameterMinor") {
                    obj->setObjectDiameterMinor(lineEdits["ellipse-diameter-minor"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_IMAGE: // \todo field editing
                break;
            case OBJ_TYPE_INFINITELINE: // \todo field editing
                break;
            case OBJ_TYPE_LINE:
                if (objName == "lineEditLineStartX") {
                    EmbVector v = to_EmbVector(obj->objectStartPoint());
                    v.x = lineEdits["line-start-x"]->text().toDouble();
                    obj->setObjectStartPoint(v);
                }
                if (objName == "lineEditLineStartY") {
                    EmbVector v = to_EmbVector(obj->objectStartPoint());
                    v.y = -lineEdits["line-start-y"]->text().toDouble();
                    obj->setObjectStartPoint(v);
                }
                if (objName == "lineEditLineEndX") {
                    EmbVector v = to_EmbVector(obj->objectStartPoint());
                    v.x = lineEdits["line-end-x"]->text().toDouble();
                    obj->setObjectEndPoint(v);
                }
                if (objName == "lineEditLineEndY") {
                    EmbVector v = to_EmbVector(obj->objectStartPoint());
                    v.y = -lineEdits["line-end-y"]->text().toDouble();
                    obj->setObjectEndPoint(v);
                }
                break;
            case OBJ_TYPE_PATH: // \todo field editing
                break;
            case OBJ_TYPE_POINT:
                if (objName == "lineEditPointX") {
                    obj->setX(lineEdits["point-x"]->text().toDouble());
                }
                if (objName == "lineEditPointY") {
                    obj->setY(-lineEdits["point-y"]->text().toDouble());
                }
                break;
            case OBJ_TYPE_POLYGON: // \todo field editing
                break;
            case OBJ_TYPE_POLYLINE: // \todo field editing
                break;
            case OBJ_TYPE_RAY: // \todo field editing
                break;
            case OBJ_TYPE_RECTANGLE: // \todo field editing
                break;
            case OBJ_TYPE_TEXTMULTI: // \todo field editing
                break;
            case OBJ_TYPE_TEXTSINGLE: // \todo field editing
                if (objName == "lineEditTextSingleContents") {
                    obj->setObjectText(lineEdits["text-single-contents"]->text());
                }
                if (objName == "comboBoxTextSingleFont") {
                    if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextFont(comboBoxTextSingleFont->currentFont().family());
                }
                if (objName == "comboBoxTextSingleJustify") {
                    if (comboBoxes["text-single-justify"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextJustify(comboBoxes["text-single-justify"]->itemData(comboBoxes["text-single-justify"]->currentIndex()).toString());
                }
                if (objName == "lineEditTextSingleHeight") {
                    obj->setObjectTextSize(lineEdits["text-single-height"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleRotation") {
                    obj->setRotation(-lineEdits["text-single-rotation"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleX") {
                    obj->setX(lineEdits["text-single-x"]->text().toDouble());
                }
                if (objName == "lineEditTextSingleY") {
                    obj->setY(lineEdits["text-single-y"]->text().toDouble());
                }
                if (objName == "comboBoxTextSingleBackward") {
                    if (comboBoxes["text-single-backward"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextBackward(comboBoxes["text-single-backward"]->itemData(comboBoxes["text-single-backward"]->currentIndex()).toBool());
                }
                if (objName == "comboBoxTextSingleUpsideDown") {
                    if (comboBoxes["text-single-upside-down"]->currentText() == fieldVariesText) {
                        break;
                    }
                    obj->setObjectTextUpsideDown(comboBoxes["text-single-upside-down"]->itemData(comboBoxes["text-single-upside-down"]->currentIndex()).toBool());
                }
                break;
            default:
                break;
        }

    }

    //Block this slot from running twice since calling setSelectedItems will trigger it
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    //Update so all fields have fresh data // \todo Improve this
    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}

#if defined(Q_OS_MAC)
/* NOTE: On Mac, if the user drops a file on the app's Dock icon,
 * or uses Open As, then this is how the app actually opens the file.
 */
class Application : public QApplication
{
public:
    Application(int argc, char **argv) {}
protected:
    /* Override the standard file open event. */
    virtual bool event(QEvent *event)
    {
        switch (event->type()) {
        case QEvent::FileOpen:
            if (_mainWin) {
                std::string file = static_cast<QFileOpenEvent *>(event)->file().toStdString();
                _mainWin->openFilesSelected({file});
                return true;
            }
            // Fall through
        default:
            return QApplication::event(event);
        }
    }
};
#endif /* MacOS */

static bool exitApp = false;

int
main(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName("Embroidermodder");
    app.setApplicationVersion(version);

    QStringList files;

    for (int i = 1; i < argc; i++) {
        QString arg(argv[i]);
        if ((arg == "-d") || (arg == "--debug")) {
        }
        else if ((arg == "-h") || (arg == "--help")) {
            fprintf(stderr, usage_msg);
            exitApp = true;
        }
        else if ((arg == "-v") || (arg == "--version")) {
            fprintf(stdout, "Embroidermodder %s\n", version);
            exitApp = true;
        }
        else if (arg == "--cov") {
            test_program = true;
        }
        else if (QFile::exists(argv[i]) && validFileFormat(arg.toStdString())) {
            files += arg;
        }
        else {
            fprintf(stderr, usage_msg);
            exitApp = true;
        }
    }

    if (exitApp) {
        return 1;
    }

    _mainWin = new MainWindow();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), _mainWin, SLOT(quit()));

    _mainWin->setWindowTitle("Embroidermodder " + app.applicationVersion());
    _mainWin->show();

    /* NOTE: If openFilesSelected() is called from within the _mainWin
     * constructor, slot commands wont work and the window menu will be screwed.
     */
    if (files.size() > 0) {
        _mainWin->openFilesSelected(files);
    }

    return app.exec();
}

