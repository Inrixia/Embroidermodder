/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2022 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 * For additional tips on our style see the linux Kernel style:
 *     https://www.kernel.org/doc/Documentation/process/coding-style.rst
 *
 */

#include <stdlib.h>

#include "core.h"

EmbVector
embVector_init(EmbReal x, EmbReal y)
{
    EmbVector v;
    v.x = x;
    v.y = y;
    return v;
}

char *
translate_str(char *str)
{
    return str;
}

void
prompt_output(char *str)
{
    puts(str);
}

int
numSelected()
{
    return 0;
}

int
deleteSelected()
{
    return 0;
}

void
alert(char *str)
{
    puts("ALERT");
    puts(str);
}

void
messageBox(char *type, char *title, char *message)
{

}

void
clear_selection()
{

}

/* Run initialisation script for this object, based on the object Type.
 *
 * Make arrowStyleAngle, arrowStyleLength. lineStyleAngle, lineStyleLength,
 * text_size defaults customizable.
 */
GeometryData*
geometry_init(int type)
{
    GeometryData *g = malloc(sizeof(GeometryData));
    g->flags = 0;

    g->arrowStyleAngle = 15.0;
    g->arrowStyleLength = 1.0;
    g->lineStyleAngle = 45.0;
    g->lineStyleLength = 1.0;
    g->text_size = 20.0;

	g->firstRun = 1;

	clear_selection();

    switch (g->mode) {

    case MODE_CIRCLE_INIT: {
        g->mode = MODE_CIRCLE_1P_RAD;
        prompt_output(translate_str(
            "Specify center point for circle or [3P/2P/TTR (tan tan radius)]: "));
        break;
    }

    case MODE_ELLIPSE_INIT: {
        g->mode = MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS;
        g->height = 1.0;
        g->width = 2.0;
        g->rotation = 0.0;
        prompt_output(translate_str("Specify first axis start point or [Center]: "));
        break;
    }

    case MODE_ERASE: {
		if (numSelected() <= 0) {
			/* TODO: Prompt to select objects if nothing is preselected. */
			alert(translate_str(
                "Preselect objects before invoking the delete command."));
			end();
			messageBox("information", translate_str("Delete Preselect"),
                translate_str("Preselect objects before invoking the delete command."));
		}
		else {
			deleteSelected();
			end();
		}
        break;
    }

    case MODE_DISTANCE: {
        prompt_output("Specify first point: ");
        break;
    }

    case MODE_DOLPHIN_INIT: {
        g->mode = MODE_DOLPHIN_NUM_POINTS;
        g->numPoints = 512;
        g->minPoints = 64;
        g->maxPoints = 8192;
        g->center = embVector_init(0.0f, 0.0f);
        g->scale = embVector_init(0.04f, 0.04f);
        /*
        add-rubber-selected POLYGON;
        set-rubber-mode POLYGON;
        update-dolphin numPoints scale_x scale_y;
        spare-rubber POLYGON;
        end();
        */
        break;
    }

    case MODE_POLYLINE_INIT: {
        g->first = embVector_init(0.0f, 0.0f);
        g->prev = embVector_init(0.0f, 0.0f);
        g->numPoints = 0;
        prompt_output(translate_str("Specify first point: "));
        break;
    }

    case MODE_SNOWFLAKE_INIT: {
        g->numPoints = 2048;
        g->minPoints = 64;
        g->maxPoints = 8192;
        g->center = embVector_init(0.0f, 0.0f);
        g->scale = embVector_init(0.04f, 0.04f);
        g->mode = MODE_SNOWFLAKE_NUM_POINTS;
        /*
        add-rubber POLYGON;
        set-rubber-mode POLYGON;
        spare_rubber POLYGON;
        end();
        */
        break;
    }

    case MODE_STAR_INIT: {
		g->numPoints = 5;
		g->center = embVector_init(0.0f, 0.0f);
		g->point1 = embVector_init(1.0f, 1.0f);
		g->point2 = embVector_init(2.0f, 2.0f);
		g->mode = MODE_STAR_NUM_POINTS;
		prompt_output(translate_str("Enter number of star points {5}: "));
        break;
	}

    default: {
        break;
    }

    }

	geometry_update(g);

    return g;
}

void
geometry_free(GeometryData *g)
{
    free(g);
}

void
set_geometry_flag(GeometryData *g, uint64_t flag)
{
    g->flags |= flag;
}

void
unset_geometry_flag(GeometryData *g, uint64_t flag)
{
    g->flags ^= flag;
}

/* Script to run on each click for this geometry object.
 *
 * The modes are documented in detail in the reference manual.
 */
void
geometry_left_click(GeometryData *geometry, EmbVector v)
{
    switch (geometry->mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            point1 = v;
            center = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", center.x, center.y);
            actuator("append-prompt");
            prompt_output(translate_str("Specify radius of circle or [Diameter]: "));
        }
        else {
            point2 = v;
            setRubberPoint("CIRCLE_RADIUS", v);
            vulcanize();
            actuator("append-prompt");
            end();
        }
        break;
    }

    case MODE_CIRCLE_1P_DIA: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            point2 = v;
            setRubberPoint("CIRCLE_DIAMETER", point2.x, point2.y);
            vulcanize();
            prompt_output();
            end();
        }
        break;
    }

    case MODE_CIRCLE_2P: {
        auto iter1 = properties.find("point1");
        auto iter2 = properties.find("point2");
        if (iter1 == properties.end()) {
            point1 = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", v);
            prompt_output();
            prompt_output(translate_str("Specify second end point of circle's diameter: ");
        }
        else if (iter2 == properties.end()) {
            point2 = node_vector(v);
            setRubberPoint("CIRCLE_TAN2", v);
            vulcanize();
            prompt_output();
            end();
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
        break;
    }

    case CIRCLE_MODE_3P: {
        if (std::isnan(point1.x)) {
            point1 = v;
            prompt_output(translate_str("Specify second point on circle: "));
        }
        else if (std::isnan(point2.x)) {
            point2.x = x;
            point2.y = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
            setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
            run_script({
                append-prompt-history;
                prompt_output-tr Specify third point on circle: "
            });
        }
        else if (std::isnan(point3.x)) {
            point3 = v;
            setRubberPoint("CIRCLE_TAN3", point3.x, point3.y);
            run_script({
                vulcanize;
                append-prompt-history;
                end"
            });
        }
    }

    case CIRCLE_MODE_TTR: {
        point1 = v;
        prompt_output();
        prompt_output(translate_str("Specify point on object for second tangent of circle: ");
        mode = MODE_TTR_SET_POINT_2;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_2: {
        point2.x = x;
        point2.y = y;
        prompt_output();
        prompt_output(translate_str("Specify radius of circle: ");
        mode = MODE_TTR_SET_POINT_3;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_3: {
        point3.x = x;
        point3.y = y;
        prompt_output();
        prompt_output(translate_str("Specify second point: ");
        mode = MODE_CIRCLE_DEFAULT;
        break;
    }

    default: {
        error("CIRCLE", tr("This should never happen."));
        break;
    }
*/

    }
}

/* Script to control the behavior of the prompt when processing
 * events for this object.
 */
void
geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH])
{
    switch (geometry->mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        if (std::isnan(point1.x)) {
            if (str == "2P") {
                mode = MODE_CIRCLE_2P;
                prompt_output(translate_str("Specify first end point of circle's diameter: ");
            }
            else if (str == "3P") {
                mode = MODE_CIRCLE_3P;
                prompt_output(translate_str("Specify first point of circle: ");
            }
            else if (str == "T" || str == "TTR") {
                mode = MODE_CIRCLE_TTR;
                prompt_output(translate_str("Specify point on object for first tangent of circle: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Point or option keyword required."));
                    prompt_output(translate_str("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ");
                }
                else {
                    x1 = std::stof(strList[0]);
                    point1.y = atof(strList[1]);
                    center.x = point1.x;
                    center.y = point1.y;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", center.x, center.y);
                    prompt_output(translate_str("Specify radius of circle or [Diameter]: ");
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                mode = MODE_CIRCLE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                prompt_output(translate_str("Specify diameter of circle: ");
            }
            else {
                EmbReal num = atof(str);
                if (std::isnan(num)) {
                    alert(translate_str("Requires numeric radius, point on circumference, or \"D\"."));
                    prompt_output(translate_str("Specify radius of circle or [Diameter]: "));
                }
                else {
                    radius = num;
                    point2.x = point1.x + radius;
                    point2.y = point1.y;
                    setRubberPoint("CIRCLE_RADIUS", point2.x, point2.y);
                    vulcanize();
                    end();
                }
            }
        }
    }

    case MODE_CIRCLE_1P_DIA: {
        if (std::isnan(point1.x)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(point2.x)) {
            EmbReal num = atof(str);
            if (std::isnan(num)) {
                alert(translate_str("Requires numeric distance or second point."));
                prompt_output(translate_str("Specify diameter of circle: "));
            }
            else {
                diameter = num;
                point2.x = point1.x + diameter.r;
                point2.y = point1.y;
                setRubberPoint("CIRCLE_DIAMETER", point2.x, point2.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_2P: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify first end point of circle's diameter: ");
            }
            else {
                x1 = atof(strList[0]);
                y1 = atof(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                prompt_output(translate_str("Specify second end point of circle's diameter: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify second end point of circle's diameter: ");
            }
            else {
                x2 = atof(strList[0]);
                y2 = atof(strList[1]);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_3P: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify first point of circle: ");
            }
            else {
                point1.x = atof(strList[0]);
                point1.y = atof(strList[1]);
                prompt_output(translate_str("Specify second point of circle: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify second point of circle: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                prompt_output(translate_str("Specify third point of circle: ");
            }
        }
        else if (std::isnan(point3.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify third point of circle: ");
            }
            else {
                point3.x = atof(strList[0]);
                point3.y = atof(strList[1]);
                setRubberPoint("CIRCLE_TAN3", point3.x, point3.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_TTR: {
        debug_message("todo CIRCLE prompt() for TTR");
        break;
    }

    default: {
        break;
    }
*/
    }
}

/* Script to change the entries in the context menu when acting
 * on this object. Also, if one is activated carry out that
 * script.
 */
void
geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH])
{
    switch (geometry->mode) {

    case MODE_CIRCLE_1P_RAD: {
        debug_message("todo CIRCLE context()");
        break;
    }

    case MODE_DISTANCE: {
        debug_message("todo DISTANCE context()");
        break;
    } 

    case MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        debug_message("TODO ELLIPSE context()");
        break;
    } 

    case MODE_MOVE: {
        debug_message("todo MOVE context()");
        break;
    } 

    case MODE_QUICKLEADER_INIT: {
        debug_message("todo QUICKLEADER context()");
        break;
    } 

    case MODE_PATH_INIT: {
        debug_message("todo PATH context()");
        break;
    } 

    case MODE_POINT_INIT: {
        debug_message("todo POINT context()");
        break;
    }

    case MODE_POLYLINE_INIT: {
        debug_message("todo POLYLINE context()");
        break;
    } 

    case MODE_RECTANGLE_INIT: {
        debug_message("todo RECTANGLE context()");
        break;
    } 

    case MODE_RGB: {
        debug_message("todo RGB context()");
        break;
    } 

    case MODE_ROTATE: {
        debug_message("todo ROTATE context()");
        break;
    } 

    case MODE_SCALE: {
        debug_message("todo SCALE context()");
        break;
    }

    case MODE_STAR_CENTER_PT: {
        debug_message("todo STAR context()");
        break;
    } 

    case MODE_TEXT_SINGLE_SETGEOM: {
        debug_message("todo SINGLELINETEXT context()");
        break;
    } 

    case MODE_POLYGON_CENTER_PT: {
        debug_message("todo POLYGON context()");
        break;
    }

    case MODE_LOCATE_POINT: {
        debug_message("todo LOCATEPOINT context()");
        break;
    }

    case MODE_LINE: {
        debug_message("todo LINE context()");
        break;
    } 

    /* . */
    default: {
        break;
    }

    }
}

/* Ensure that generated data is correct by running this before operations that
 * rely on it.
 */
void
geometry_update(GeometryData *g)
{

}

#if 0

/* . */
void
Geometry::distance_click(EmbVector v)
{
    if (std::isnan(point1.x)) {
        point1 = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", point1.x, point1.y);
        prompt_output();
        prompt_output(translate_str("Specify second point: "));
    }
    else {
        prompt_output();
        point2 = v;
        reportDistance();
        end();
    }
}


/* . */
void
Geometry::distance_prompt(String args)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Requires numeric distance or two points."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            point1.x = atof(strList[0]);
            point1.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", point1.x, point1.y);
            prompt_output(translate_str("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Requires numeric distance or two points."));
            prompt_output(translate_str("Specify second point: "));
        }
        else {
            point2.x = atof(strList[0]);
            point2.y = atof(strList[1]);
            reportDistance();
            end();
        }
    }
}

/*
 * Cartesian Coordinate System reported:
 *
 *               (+)
 *               90
 *               |
 *      (-) 180__|__0 (+)
 *               |
 *              270
 *              (-)
 */
void
Geometry::reportDistance()
{
    EmbVector delta;
    EmbReal dx = point2.x - point1.x;
    EmbReal dy = point2.y - point1.y;

    EmbReal dist = calculateDistance(point1.x, point1.y, point2.x, point2.y);
    EmbReal angle = calculateAngle(point1.x, point1.y, point2.x, point2.y);

    prompt_output("" + tr("Distance") + " = " + dist.toString() + ", " + tr("Angle") + " = " + angle.toString());
    prompt_output();
    prompt_output("" + tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    prompt_output();
}

/* Update the dolphin object. */
void
Geometry::update_dolphin(int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0 * CONSTANT_PI) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/* . */
void
Geometry::ellipse_click(EmbVector v)
{
    case MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            point1 = v;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
            prompt_output();
            prompt_output(translate_str("Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2 = v;
            center.x = atof((point1.x + point2.x)/2.0);
            center.y = atof((point1.y + point2.y)/2.0);
            width = calculateDistance(point1.x, point1.y, point2.x, point2.y);
            rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output();
            prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
            prompt_output();
            end();
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            point1.x = x;
            point1.y = y;
            center.x = point1.x;
            center.y = point1.y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
            prompt_output();
            prompt_output(translate_str("Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2.x = x;
            point2.y = y;
            width = calculateDistance(center.x, center.y, point2.x, point2.y)*2.0;
            rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output();
            prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, center.x, center.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
            prompt_output();
            end();
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            EmbReal angle = calculateAngle(center.x, center.y, x, y);
            height = cos(angle*PI/180.0)*width;
            addEllipse(center.x, center.y, width, height, rot, 0);
            prompt_output();
            end();
        }
    }
}

/* . */
void
Geometry::ellipse_prompt(String args)
{
    switch (mode) {
    case MODE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            if (str == "C" || str == "CENTER") {
                mode = MODE_MAJORRADIUS_MINORRADIUS;
                prompt_output(translate_str("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Point or option keyword required."));
                    prompt_output(translate_str("Specify first axis start point or [Center]: ");
                }
                else {
                    point1.x = atof(strList[0]);
                    point1.y = atof(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                    prompt_output(translate_str("Specify first axis end point: ");
                }
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify first axis end point: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                center.x = (point1.x + point2.x)/2.0;
                center.y = (point1.y + point2.y)/2.0;
                width = calculateDistance(point1.x, point1.y, point2.x, point2.y);
                rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate_str("Specify rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Point or option keyword required."));
                    prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    end();
                }
            }
        }
        break;
    }
    case MODE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify center point: ");
            }
            else {
                point1.x = atof(strList[0]);
                point1.y = atof(strList[1]);
                center.x = point1.x;
                center.y = point1.y;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
                prompt_output(translate_str("Specify first axis end point: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify first axis end point: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                width = calculateDistance(point1.x, point1.y, point2.x, point2.y)*2.0;
                rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate_str("Specify ellipse rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Point or option keyword required."));
                    prompt_output(translate_str("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    end();
                }
            }
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            if (std::isnan(str)) {
                alert(translate_str("Invalid angle. Input a numeric angle or pick a point."));
                prompt_output(translate_str("Specify rotation: ");
            }
            else {
                EmbReal angle = atof(str);
                height = cos(angle*PI/180.0)*width;
                addEllipse(center.x, center.y, width, height, rot, 0);
                end();
            }
        }
        break;
    }
    default:
        break;
    }
}

/* . */
void
Geometry::heart_main(void)
{
    numPoints = node_int(512); //Default //TODO: min:64 max:8192
    center.x = 0.0f;
    center.y = 0.0f;
    scale.x = node(1.0f);
    scale.y = node(1.0f);
    clear_selection();;
    mode = node_str("MODE_NUM_POINTS");

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", numPoints, scale.x, scale.y);
    spareRubber("POLYGON");
    end();
}

/* . */
void
Geometry::updateHeart(String style, int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbVector v;
        EmbReal t = (2.0*CONSTANT_PI)/numPoints*i;

        if (style == "HEART4") {
            v.x = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            v.y = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5") {
            v.x = 16*pow(sin(t), 3);
            v.y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        v.x *= xScale;
        v.y *= yScale;
        setRubberPoint("POLYGON_POINT_" + i.toString(), v);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/* . */
void
Geometry::line_main(void)
{
    clear_selection();
    g->firstRun = 1;
    g->first = {0.0f, 0.0f};
    g->prev = {0.0f, 0.0f};
    prompt_output(translate_str("Specify first point: ");
}

/* . */
void
Geometry::line_click(GeometryData *g, EmbVector v)
{
    if (firstRun) {
        g->firstRun = 0;
        g->first = v;
        g->prev = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", g->first);
        prompt_output(translate_str("Specify next point or [Undo]: ");
    }
    else {
        setRubberPoint("LINE_END", v);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", v);
        prompt_output();
        prev = v;
    }
}


/* . */
void
Geometry::line_prompt(String args)
{
    if (firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            firstRun = 0;
            g->first = get_vector(strList);
            g->prev = g->first;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", g->first);
            prompt_output(translate_str("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            debug_message("todo LINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Point or option keyword required."));
                prompt_output(translate_str("Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = atof(strList[0]);
                EmbReal y = atof(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                prev.x = x;
                properties["prevY"] = y;
                prompt_output(translate_str("Specify next point or [Undo]: ");
            }
        }
    }
}

/* locate_point_main */
void
Geometry::locate_point_main(void)
{
	clear-selection();
	prompt_output(translate_str("Specify point: \n"));
}

/* locate_point_click v */
void
Geometry::locate_point_click(EmbVector v)
{
    prompt_output();
    prompt_output("v.x = " + v.x + ", Y = " + v.y);
    prompt_output();
    end();
}


/* . */
void
Geometry::locate_point_prompt(String args)
{
    std::vector<std::string> strList = tokenize(args, ',');
    if (std::isnan(strList[0]) || std::isnan(strList[1])) {
        alert(translate_str("Invalid point."));
        prompt_output(translate_str("Specify point: ");
    }
    else {
        prompt_output("X = " + strList[0] + ", Y = " + strList[1]);
        end();
    }
}

/* . */
void
Geometry::move_main(void)
{
    init();
    firstRun.b = node_int(1);
    base.x = 0.0f;
    base.y = 0.0f;
    dest.x = 0.0f;
    dest.y = 0.0f;
    delta.x = 0.0f;
    delta.y = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate_str("Preselect objects before invoking the move command."));
        end();
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate_str("Specify base point: "));
    }
}

/* . */
void
Geometry::move_click(EmbVector v)
{
    if (firstRun) {
        firstRun = 0;
        base = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", base);
        previewOn("SELECTED", "MOVE", base, 0);
        prompt_output(translate_str("Specify destination point: ");
    }
    else {
        dest = v;
        delta = embVector_subtract(dest, base);
        moveSelected(delta.x, delta.y);
        previewOff();
        end();
    }
}

/* . */
void
Geometry::move_prompt(String str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify base point: ");
        }
        else {
            firstRun = node_int(0);
            base.x = atof(strList[0]);
            base.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", baseX, baseY);
            previewOn("SELECTED", "MOVE", baseX, baseY, 0);
            prompt_output(translate_str("Specify destination point: ");
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify destination point: ");
        }
        else {
            dest.x = atof(strList[0]);
            dest.y = atof(strList[1]);
            delta.x = dest.x - base.x;
            delta.y = dest.y - base.y;
            moveSelected(deltaX, deltaY);
            previewOff();
            end();
        }
    }
}

/* TODO: The path command is currently broken. */
void
Geometry::path_main(void)
{
    firstRun = node_int(1);
    first = {0.0, 0.0};
    prev = {0.0, 0.0};
    init();
    clear_selection();;
    firstRun = atof(1);
    prompt_output(translate_str("Specify start point: ");
}

/* . */
void
Geometry::path_click(EmbReal x, EmbReal y)
{
    if (firstRun.b) {
        firstRun = 0;
        first.x = x;
        first.y = y;
        prev.x = x;
        prev.y = y;
        addPath(x, y);
        prompt_output();
        prompt_output(translate_str("Specify next point or [Arc/Undo]: ");
    }
    else {
        prompt_output();
        appendLineToPath(x, y);
        prev.x = x;
        prev.y = y;
    }
}


/* . */
void
Geometry::path_prompt(String args)
{
    if (str == "A" || str == "ARC") {
        debug_message("todo PATH prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        debug_message("todo PATH prompt() for UNDO");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Point or option keyword required."));
            prompt_output(translate_str("Specify next point or [Arc/Undo]: ");
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            if (firstRun) {
                firstRun = 0;
                first = embVector(x, y);
                prev = embVector(x, y);
                addPath(x, y);
                prompt_output(translate_str("Specify next point or [Arc/Undo]: ");
            }
            else {
                appendLineToPath(x, y);
                prev = embVector(x, y);
            }
        }
    }
}

/* . */
void
Geometry::point_main(void)
{
    init();
    clear_selection();;
    firstRun = 1;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    prompt_output();
    prompt_output(translate_str("Specify first point: ");
}

/* . */
void
Geometry::point_click(EmbVector v)
{
    if (firstRun) {
        firstRun = 0;
        prompt_output();
        prompt_output(translate_str("Specify next point: "));
        addPoint(x, y);
    }
    else {
        prompt_output();
        addPoint(x, y);
    }
}

/* . */
void
Geometry::point_prompt(String str)
{
    if (firstRun) {
        if (str == "M" || str == "MODE") {
            debug_message("todo POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            debug_message("todo POINT prompt() for PDSIZE");
        }
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            firstRun = 0;
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            prompt_output(translate_str("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify next point: "));
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            prompt_output(translate_str("Specify next point: "));
            addPoint(x,y);
        }
    }
}

/* . */
void
Geometry::polygon_main(void)
{
    init();
    clear_selection();;
    center = embVector_init(0.0f, 0.0f);
    side1 = embVector_init(0.0f, 0.0f);
    side2 = embVector_init(0.0f, 0.0f);
    pointI = embVector_init(0.0f, 0.0f);
    pointC = embVector_init(0.0f, 0.0f);
    polyType = "Inscribed"; //Default
    numSides = 4;           //Default
    mode = MODE_NUM_SIDES;
    prompt_output(translate_str("Enter number of sides" + " {" + numSides.toString() + "}: ");
}

/* . */
void
Geometry::polygon_click(EmbVector v)
{
    switch (mode) {
    case MODE_POLYGON_NUM_SIDES:
    case MODE_POLYGON_POLYTYPE:
    case MODE_POLYGON_DISTANCE:
        //Do nothing, the prompt controls this.
        break;
    case MODE_POLYGON_CENTER_PT: {
        center = v;
        mode = POLYGON_MODE_POLYTYPE;
        prompt_output();
        prompt_output(translate_str("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        break;
    }
    case MODE_POLYGON_INSCRIBE: {
        pointI = v;
        setRubberPoint("POLYGON_INSCRIBE_POINT", pointI.x, pointI.y);
        vulcanize();
        prompt_output();
        end();
        break;
    }
    case MODE_POLYGON_CIRCUMSCRIBE: {
        pointC = v;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX"], pointC.y);
        vulcanize();
        prompt_output();
        end();
        break;
    }
    case MODE_POLYGON_SIDE_LEN: {
        debug_message("todo POLYGON Sidelength mode");
        break;
    }
}


/* Polygon */
void
Geometry::polygon_prompt(String str)
{
    case MODE_POLYGON_NUM_SIDES: {
        if (str == "" && numSides >= 3 && numSides <= 1024) {
            prompt_output(translate_str("Specify center point or [Sidelength]: ");
            mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate_str("Requires an integer between 3 and 1024."));
                prompt_output(translate_str("Enter number of sides" + " {" + numSides.toString() + "}: ");
            }
            else {
                numSides = tmp;
                prompt_output(translate_str("Specify center point or [Sidelength]: "));
                mode = MODE_CENTER_PT;
            }
        }
        break;
    }
    case MODE_POLYGON_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            mode = MODE_SIDE_LEN;
            prompt_output(translate_str("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Point or option keyword required."));
                prompt_output(translate_str("Specify center point or [Sidelength]: "));
            }
            else {
                center.x = atof(strList[0]);
                center.y = atof(strList[1]);
                mode = MODE_POLYTYPE;
                prompt_output(translate_str("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            mode = MODE_INSCRIBE;
            polyType = "Inscribed";
            prompt_output(translate_str("Specify polygon corner point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            mode = MODE_CIRCUMSCRIBE;
            polyType = "Circumscribed";
            prompt_output(translate_str("Specify polygon side point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "") {
            if (polyType == "Inscribed") {
                mode = MODE_INSCRIBE;
                prompt_output(translate_str("Specify polygon corner point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", center.x, center.y);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else if (polyType == "Circumscribed") {
                mode = MODE_CIRCUMSCRIBE;
                prompt_output(translate_str("Specify polygon side point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", center.x, center.y);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(translate_str("Invalid option keyword."));
            prompt_output(translate_str("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        }
    }
    case MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate_str("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Point or option keyword required."));
                prompt_output(translate_str("Specify polygon corner point or [Distance]: "));
            }
            else {
                pointI.x = atof(strList[0]);
                pointI.y = atof(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end();
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate_str("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Point or option keyword required."));
                prompt_output(translate_str("Specify polygon side point or [Distance]: "));
            }
            else {
                pointC.x = atof(strList[0]);
                pointC.y = atof(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end();
            }
        }
    }
    case MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(translate_str("Requires valid numeric distance."));
            prompt_output(translate_str("Specify distance: "));
        }
        else {
            if (polyType == "Inscribed") {
                pointI.x = center.x;
                pointI.y = center.y + atof(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end();
            }
            else if (polyType == "Circumscribed") {
                pointC.x = center.x;
                pointC.y = center.y + atof(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end();
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    case MODE_SIDE_LEN: {
        debug_message("todo POLYGON Sidelength mode");
        break;
    }
}

/* . */
void
Geometry::polyline_main(void)
{
    script(polyline_init);
}

/* . */
void
Geometry::polyline_click(EmbVector v)
{
    if (firstRun.b) {
        firstRun = 0;
        first = v;
        prev = v;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", first);
        prompt_output();
        prompt_output(translate_str("Specify next point or [Undo]: ");
    }
    else {
        num++;
        setRubberPoint("POLYLINE_POINT_" + num.toString(), v);
        setRubberText("POLYLINE_NUM_POINTS", num.toString());
        spareRubber("POLYLINE");
        prompt_output();
        prev = v;
    }
}

/* . */
void
Geometry::polyline_prompt(String str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            firstRun = 0;
            first.x = atoi(strList[0]);
            first.y = atoi(strList[1]);
            prev.x = firstX;
            prev.y = first.y;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", firstX, first.y);
            prompt_output(translate_str("Specify next point or [Undo]: ");
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            debug_message("todo POLYLINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Point or option keyword required."));
                prompt_output(translate_str("Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = atof(strList[0]);
                EmbReal y = atof(strList[1]);
                properties["num"] = properties["num"].i + 1;
                setRubberPoint("POLYLINE_POINT_" + properties["num"].toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", properties["num"].toString());
                spareRubber("POLYLINE");
                prev.x = x;
                prev.y = y;
                prompt_output(translate_str("Specify next point or [Undo]: ");
            }
        }
    }
}

const char quickleader_main[][MAX_STRING_LENGTH] = {
    todo Adding the text is not complete yet.;
    init();
    clear_selection();
    x[0] = 0.0f;
    y[0] = 0.0f;
    x[1] = 0.0f;
    y[1] = 0.0f;
    prompt_output-tr Specify first point: "
};

/* . */
void
Geometry::quickleader_click(EmbVector v)
{
    if (std::isnan(point1.x)) {
        point1.x = v.x;
        point1.y = v.y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
        prompt_output();
        prompt_output(translate_str("Specify second point: "));
    }
    else {
        point2.x = v.x;
        point2.y = v.y;
        setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
        vulcanize();
        prompt_output();
        end();
    }
}

/* . */
void
Geometry::quickleader_prompt(String str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Requires two points."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            point1.x = atoi(strList[0]);
            point1.y = atoi(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
            prompt_output(translate_str("Specify second point: ");
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Requires two points."));
            prompt_output(translate_str("Specify second point: ");
        }
        else {
            point2.x = atoi(strList[0]);
            point2.y = atoi(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
            vulcanize();
            end();
        }
    }
}

/* . */
const char rectangle_click_script[][MAX_STRING_LENGTH] = {
    if (properties["newRect"].b) {
        properties["newRect"].b = 0;
        point1.x = v.x;
        point1.y = v.y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        prompt_output(translate_str("Specify other corner point or [Dimensions]: "));
    }
    else {
        newRect = 1;
        x2 = v.x;
        y2 = v.y;
        setRubberPoint RECTANGLE_END x y;
        vulcanize;
        end"
    }
};

/* . */
void
Geometry::rectangle_prompt(String str)
{
    if (str == "C" || str == "CHAMFER") {
        debug_message("todo RECTANGLE prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        debug_message("todo RECTANGLE prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        debug_message("todo RECTANGLE prompt() for FILLET");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify first point: "));
        }
        else {
            EmbReal x = atoi(strList[0]);
            EmbReal y = atoi(strList[1]);
            if (properties["newRect"].b) {
                properties["newRect"] = 0;
                point1.x = x;
                point1.y = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                prompt_output(translate_str("Specify other corner point or [Dimensions]: "));
            }
            else {
                properties["newRect"] = 1;
                point2.x = x;
                point2.y = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                end();
            }
        }
    }
}

/* . */
void
Geometry::rgb_main(void)
{
    init();
    clear_selection();;
    actuator("mode=RGB_MODE_BACKGROUND");
    prompt_output(translate_str("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

/* . */
void
Geometry::rgb_click(EmbVector v)
{
    //Do Nothing, prompt only command.
}

/* . */
void
Geometry::rgb_prompt(String str)
{
    if (mode == "RGB_MODE_BACKGROUND") {
        if (str == "C" || str == "CROSSHAIR") {
            mode = atoi("RGB_MODE_CROSSHAIR");
            prompt_output(translate_str("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            mode = RGB_MODE_GRID;
            prompt_output(translate_str("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = atoi(strList[0]);
            EmbReal g = atoi(strList[1]);
            EmbReal b = atoi(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(translate_str("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate_str("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                end();
            }
        }
    }
    case RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = atoi(strList[0]);
        EmbReal g = atoi(strList[1]);
        EmbReal b = atoi(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate_str("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate_str("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            end();
        }
    }
    case RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = atof(strList[0]);
        EmbReal g = atof(strList[1]);
        EmbReal b = atof(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate_str("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate_str("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            end();
        }
    }
}

/* . */
void
Geometry::rotate_main(String args)
{
    init();
    mode = ROTATE_MODE_NORMAL;
    firstRun = 1;
    base = {0.0f, 0.0f};
    dest = {0.0f, 0.0f};
    angle = 0.0f;

    properties["baseR"] = {0.0f, 0.0f};
    properties["destR"] = {0.0f, 0.0f};
    angleRef = 0.0f;
    angleNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate_str("Preselect objects before invoking the rotate command."));
        end();
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate_str("Specify base point: "));
    }
}

/* . */
void
Geometry::rotate_click(EmbVector v)
{
    switch (mode) {
    case ROTATE_MODE_NORMAL:
        if (firstRun) {
            firstRun = 0;
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, 0);
            prompt_output();
            prompt_output(translate_str("Specify rotation angle or [Reference]: "));
        }
        else {
            dest = v;
            angle = calculateAngle(baseX, baseY, dest.x, dest.y);
            prompt_output();
            rotateSelected(baseX, baseY, angle);
            previewOff();
            end();
        }
    }
    case ROTATE_MODE_REFERENCE) {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            prompt_output();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
            prompt_output(translate_str("Specify second point: ");
        }
        else if (std::isnan(destRX)) {
            properties["destR"] = v;
            angleRef = calculateAngle(properties["baseRX"], properties["baseRY"], destRX, destRY);
            setRubberPoint("LINE_START", baseX, baseY);
            previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
            prompt_output();
            prompt_output(translate_str("Specify the new angle: ");
        }
        else if (std::isnan(angleNew)) {
            angleNew = calculateAngle(baseX, baseY, x, y);
            rotateSelected(baseX, baseY, angleNew - angleRef);
            previewOff();
            end();
        }
    }
}

/* . */
void
Geometry::rotate_prompt(String str)
{
    if (mode == ROTATE_MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify base point: ");
            }
            else {
                firstRun = 0;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, 0);
                prompt_output(translate_str("Specify rotation angle or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate_str("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(translate_str("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate_str("Specify rotation angle or [Reference]: ");
                }
                else {
                    angle = atof(str);
                    rotateSelected(baseX, baseY, angle);
                    previewOff();
                    end();
                }
            }
        }
    }
    case MODE_REFERENCE) {
        if (std::isnan(properties["baseRX"])) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric angle or two points."));
                    prompt_output(translate_str("Specify the reference angle {0.00}: ");
                }
                else {
                    properties["baseR.x"] = atof(strList[0]);
                    properties["baseR.y"] = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
                    prompt_output(translate_str("Specify second point: ");
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference angle is what we will use later.
                angleRef = atof(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                prompt_output(translate_str("Specify the new angle: ");
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric angle or two points."));
                    prompt_output(translate_str("Specify second point: "));
                }
                else {
                    properties["destR.x"] = atof(strList[0]);
                    properties["destR.y"] = atof(strList[1]);
                    angleRef = calculateAngle(properties["baseRX"], properties["baseRY"], destRX, destRY);
                    previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                    setRubberPoint("LINE_START", baseX, baseY);
                    prompt_output(translate_str("Specify the new angle: ");
                }
            }
            else {
                // The base and dest values are only set here to advance the command.
                baseR = {0.0, 0.0};
                destR = {0.0, 0.0};
                // The reference angle is what we will use later.
                angleRef = atof(str);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                prompt_output(translate_str("Specify the new angle: ");
            }
        }
        else if (std::isnan(angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric angle or second point."));
                    prompt_output(translate_str("Specify the new angle: ");
                }
                else {
                    EmbVector v;
                    v.x = atof(strList[0]);
                    v.y = atof(strList[1]);
                    angleNew = calculateAngle(base, v);
                    rotateSelected(base, angleNew - angleRef);
                    previewOff();
                    end();
                }
            }
            else {
                angleNew = atof(str);
                rotateSelected(base, angleNew - angleRef);
                previewOff();
                end();
            }
        }
    }
}

//Command: Sandbox

/*String properties;
properties["test1"];
properties["test2"];
*/

/* . */
void
Geometry::sandbox_main(String str)
{
    init();

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    prompt_output();

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    offset.x = 0.0;
    offset.y = 0.0;

    EmbReal polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);

    offset.x = 5.0;
    offset.y = 0.0;

    EmbReal polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);

    end();
}

/* Command: Scale. */
void
Geometry::scale_main(void)
{
    init();

    mode = MODE_NORMAL;
    firstRun = 1;
    base = {0.0f, 0.0f};
    dest = {0.0f, 0.0f};
    factor = 0.0f;

    baseR = base;
    destR = dest;
    factorRef = 0.0f;
    factorNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate_str("Preselect objects before invoking the scale command."));
        end();
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate_str("Specify base point: "));
    }
}

/* . */
void
Geometry::scale_click(EmbVector v)
{
    switch (mode) {
    case SCALE_MODE_NORMAL: {
        if (firstRun) {
            firstRun = 0;
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", base);
            previewOn("SELECTED", "SCALE", base, 1);
            prompt_output();
            prompt_output(translate_str("Specify scale factor or [Reference]: "));
        }
        else {
            dest = v;
            factor = calculateDistance(base, dest);
            prompt_output();
            scaleSelected(base, factor);
            previewOff();
            end();
        }
        break;
    }
    case MODE_REFERENCE") {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            prompt_output();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseR"]);
            prompt_output(translate_str("Specify second point: "));
        }
        else if (std::isnan(destRX)) {
            properties["destR"] = v;
            factorRef = calculateDistance(properties["baseRX"].r, properties["baseRY"].r, destRX, destRY);
            if (factorRef <= 0.0) {
                destRX = 0.0f;
                destRY = 0.0f;
                factorRef = 0.0f;
                alert(translate_str("Value must be positive and nonzero."));
                prompt_output(translate_str("Specify second point: "));
            }
            else {
                prompt_output();
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                prompt_output(translate_str("Specify new length: ");
            }
        }
        else if (std::isnan(factorNew)) {
            factorNew = calculateDistance(baseX, baseY, x, y);
            if (factorNew <= 0.0) {
                factorNew = 0.0f;
                alert(translate_str("Value must be positive and nonzero."));
                prompt_output(translate_str("Specify new length: ");
            }
            else {
                prompt_output();
                scaleSelected(baseX, baseY, factorNew/factorRef);
                previewOff();
                end();
            }
        }
    }
}

/* . */
void
Geometry::scale_prompt(String str)
{
    if (mode == MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate_str("Invalid point."));
                prompt_output(translate_str("Specify base point: ");
            }
            else {
                firstRun = 0;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, 1);
                prompt_output(translate_str("Specify scale factor or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate_str("Specify reference length {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(translate_str("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate_str("Specify scale factor or [Reference]: ");
                }
                else {
                    factor = atof(str);
                    scaleSelected(base.x, base.y, factor);
                    previewOff();
                    end();
                }
            }
        }
    }
    case MODE_REFERENCE") {
        if (std::isnan(properties["baseRX"].r)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric distance or two points."));
                    prompt_output(translate_str("Specify reference length {1}: ");
                }
                else {
                    properties["baseR.x"] = atof(strList[0]);
                    properties["baseR.y"] = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
                    prompt_output(translate_str("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    properties["baseRX"] = 0.0f;
                    properties["baseRY"] = 0.0f;
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(translate_str("Value must be positive and nonzero."));
                    prompt_output(translate_str("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                    prompt_output(translate_str("Specify new length: "));
                }
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric distance or two points."));
                    prompt_output(translate_str("Specify second point: "));
                }
                else {
                    properties["destR.x"] = atof(strList[0]);
                    properties["destR.y"] = atof(strList[1]);
                    properties["factorRef = calculateDistance(properties["baseRX, properties["baseRY"], destRX, destRY);
                    if (factorRef <= 0.0) {
                        destRX = 0.0f;
                        destRY = 0.0f;
                        factorRef = 0.0f;
                        alert(translate_str("Value must be positive and nonzero."));
                        prompt_output(translate_str("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", baseX, baseY);
                        previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                        prompt_output(translate_str("Specify new length: ");
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                // The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(translate_str("Value must be positive and nonzero."));
                    prompt_output(translate_str("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", baseX, baseY);
                    previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                    prompt_output(translate_str("Specify new length: "));
                }
            }
        }
        else if (std::isnan(factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Requires valid numeric distance or second point."));
                    prompt_output(translate_str("Specify new length: "));
                }
                else {
                    EmbReal x = atof(strList[0]);
                    EmbReal y = atof(strList[1]);
                    factorNew = calculateDistance(baseX, baseY, x, y);
                    if (factorNew <= 0.0) {
                        factorNew = 0.0f;
                        alert(translate_str("Value must be positive and nonzero."));
                        prompt_output(translate_str("Specify new length: "));
                    }
                    else {
                        scaleSelected(baseX, baseY, factorNew/factorRef);
                        previewOff();
                        end();
                    }
                }
            }
            else {
                factorNew = atof(str);
                if (factorNew <= 0.0) {
                    factorNew = 0.0f;
                    alert(translate_str("Value must be positive and nonzero."));
                    prompt_output(translate_str("Specify new length: "));
                }
                else {
                    scaleSelected(properties["baseX, properties["baseY, properties["factorNew/properties["factorRef);
                    previewOff();
                    end();
                }
            }
        }
    }
}

/* . */
void
Geometry::text_single_main(void)
{
    init();
    clear_selection();;
    g->text = "";
    g->text.x = 0.0f;
    g->text.y = 0.0f;
    g->text.justify = atof("Left");
    g->textFont = textFont();
    g->textHeight = 0.0f;
    g->textRotation = 0.0f;
    mode = MODE_TEXT_SINGLE_SETGEOM;
    prompt_output(translate_str("Current font: ") + "{"
        + properties["textFont"].s + "} " + tr("Text height: ") + "{"
        + textSize() + "}");
    prompt_output(translate_str("Specify start point of text or [Justify/Setfont]: ");
}

/* . */
void
Geometry::text_single_click(EmbVector v)
{
    switch (mode) {

    case MODE_TEXT_SINGLE_SETGEOM: {
        if (std::isnan(textX)) {
            g->position = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", v);
            prompt_output();
            prompt_output(translate_str("Specify text height" + " {" + textSize() + "}: ");
        }
        else if (std::isnan(properties["textHeight"].r)) {
            properties["textHeight"] = calculateDistance(textX, textY, x, y);
            setTextSize(properties["textHeight"].r);
            prompt_output();
            prompt_output(translate_str("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(properties["textRotation"].r)) {
            properties["textRotation"] = calculateAngle(textX, textY, x, y);
            setTextAngle(properties["textRotation"].r);
            prompt_output();
            prompt_output(translate_str("Enter text: "));
            mode = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", textX, textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
            setRubberText("TEXT_FONT", properties["textFont"].r);
            setRubberText("TEXT_JUSTIFY", textJustify);
            setRubberText("TEXT_RAPID", properties["text"].r);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }

    default:
        break;
    }
}

/* . */
void
Geometry::text_single_prompt(String str)
{
    case MODE_JUSTIFY: {
        if (str == "C" || str == "CENTER") {
            mode = MODE_SETGEOM;
            textJustify = node("Center");
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify center point of text or [Justify/Setfont]: ");
        }
        else if (str == "R" || str == "RIGHT") {
            mode = MODE_SETGEOM;
            textJustify = node("Right");
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            mode = MODE_SETGEOM;
            textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            mode = MODE_SETGEOM;
            textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify middle point of text or [Justify/Setfont]: ");
        }
        else if (str == "F" || str == "FIT") {
            mode = MODE_SETGEOM;
            textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify middle-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify middle-center point of text or [Justify/Setfont]: ");
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify middle-right point of text or [Justify/Setfont]: ");
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify bottom-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate_str("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate_str("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
        }
    }
    case MODE_SETFONT: {
        mode = MODE_SETGEOM;
        textFont = str;
        setRubberText("TEXT_FONT", textFont);
        setTextFont(properties["textFont"].s);
        prompt_output(translate_str("Specify start point of text or [Justify/Setfont]: ");
    }
    case MODE_SETGEOM: {
        if (std::isnan(textX)) {
            if (str == "J" || str == "JUSTIFY") {
                mode = "MODE_JUSTIFY";
                prompt_output(translate_str("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
            }
            else if (str == "S" || str == "SETFONT") {
                mode = MODE_SETFONT;
                prompt_output(translate_str("Specify font name: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate_str("Point or option keyword required."));
                    prompt_output(translate_str("Specify start point of text or [Justify/Setfont]: ");
                }
                else {
                    g->position = get_vector(strList);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->position);
                    prompt_output(translate_str("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(properties["textHeight"].r)) {
            if (str == "") {
                properties["textHeight"] = textSize();
                prompt_output(translate_str("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(translate_str("Requires valid numeric distance or second point."));
                prompt_output(translate_str("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                properties["textHeight"] = atof(str);
                setTextSize(properties["textHeight"]);
                prompt_output(translate_str("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(properties["textRotation"])) {
            if (str == "") {
                properties["textRotation"] = textAngle();
                prompt_output(translate_str("Enter text: ");
                mode = "MODE_RAPID";
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", textX, textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
                setRubberText("TEXT_FONT", properties["textFont"].s);
                setRubberText("TEXT_JUSTIFY", textJustify);
                setRubberText("TEXT_RAPID", properties["text"].s);
            }
            else if (std::isnan(str)) {
                alert(translate_str("Requires valid numeric angle or second point."));
                prompt_output(translate_str("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                properties["textRotation"] = atof(str);
                setTextAngle(properties["textRotation"].r);
                prompt_output(translate_str("Enter text: ");
                mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", textX, textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"], properties["textRotation"]);
                setRubberText("TEXT_FONT", properties["textFont"]);
                setRubberText("TEXT_JUSTIFY", textJustify);
                setRubberText("TEXT_RAPID", properties["text"]);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    case MODE_RAPID: {
        if (str == "RAPID_ENTER") {
            if (properties["text"] == "") {
                end();
            }
            else {
                vulcanize();
                end();
                /* TODO: Rather than ending the command, calculate where the
                 * next line would be and modify the x/y to the new point.
                 */
            }
        }
        else {
            properties["text"] = atof(str);
            setRubberText("TEXT_RAPID", properties["text"].s);
        }
    }
}

/* . */
void
Geometry::snowflake_main(void)
{
    object_script(snowflake_init);
}

/* Snowflake Curve with $t \in [0, 2\pi]$. */
void
Geometry::update_snowflake(void)
{
    EmbVector scale;
    scale.x = properties["scale.x"].r;
    scale.y = properties["scale.y"].r;
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0*CONSTANT_PI) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, snowflake_x);
        v.y = fourier_series(t, snowflake_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), v.x*scale.x, v.y*scale.y);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}


/* . */
void
Geometry::star_click(EmbReal mouse)
{
    switch (mode) {

    case MODE_STAR_NUM_POINTS: {
        /* Do nothing, the prompt controls this. */
        break;
    }

    case MODE_STAR_CENTER_PT: {
        center = mouse;
        mode = STAR_MODE_RAD_OUTER;
        prompt_output(translate_str("Specify outer radius of star: ");
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(properties, center);
        actuator("enable move-rapid-fire");
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        point1 = mouse;
        mode.s = "MODE_RAD_INNER";
        prompt_output(translate_str("Specify inner radius of star: ");
        updateStar(point1);
        break;
    }

    case MODE_STAR_RAD_INNER: {
        point2 = mouse;
        actuator("disable move-rapid-fire");
        updateStar(point2);
        spareRubber("POLYGON");
        end();
        break;
    }

    default: {
        break;
    }

    }
}

/* . */
const char star_move[][] = {
    (EmbVector v)
    switch (mode) {
    case MODE_STAR_NUM_POINTS: {
        //Do nothing, the prompt controls this.
        break;
    }
    case MODE_STAR_CENTER_PT: {
        //Do nothing, prompt and click controls this.
        break;
    }
    case MODE_STAR_RAD_OUTER: {
        properties = updateStar(properties, v);
        break;
    }
    case MODE_STAR_RAD_INNER: {
        properties = updateStar(properties, v);
        break;
    }
};


/* . */
void
Geometry::star_prompt(String str)
{
    switch (mode) {
    case STAR_MODE_NUM_POINTS: {
        if (str == "" && numPoints >= 3 && numPoints <= 1024) {
            prompt_output(translate_str("Specify center point: ");
            mode = MODE_STAR_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate_str("Requires an integer between 3 and 1024."));
                prompt_output(translate_str("Enter number of star points") + " {" + numPoints.toString() + "}: ");
            }
            else {
                numPoints = tmp;
                prompt_output(translate_str("Specify center point: ");
                mode = MODE_STAR_CENTER_PT;
            }
        }
        break;
    }

    case MODE_STAR_CENTER_PT: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify center point: ");
        }
        else {
            center.x = atof(strList[0]);
            center.y = atof(strList[1]);
            mode = MODE_STAR_RAD_OUTER;
            prompt_output(translate_str("Specify outer radius of star: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            actuator("enable move-rapid-fire");
        }
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify outer radius of star: ");
        }
        else {
            point1 = get_vector(strList);
            mode = MODE_RAD_INNER;
            prompt_output(translate_str("Specify inner radius of star: ");
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }

    case MODE_STAR_RAD_INNER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate_str("Invalid point."));
            prompt_output(translate_str("Specify inner radius of star: ");
        }
        else {
            point2 = get_vector(strList);
            actuator("disable move-rapid-fire");
            updateStar();
            spareRubber("POLYGON");
            end();
        }
        break;
    }

    default:
        break;
    }

}

/* Update star. */
void
Geometry::updateStar(EmbVector mouse)
{
    EmbVector v = mouse - center;
    EmbReal angOuter = embVector_angle(v);
    EmbReal distOuter = embVector_length(v);
    EmbReal distInner = distOuter/2.0;

    if (mode == MODE_STAR_RAD_INNER) {
        EmbVector v = point1 - center;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = embVector_distance(center, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(numPoints*2);
    for (int i = 0; i < numPoints*2; i++) {
        EmbReal angle = (angOuter + (angInc*i)) * CONSTANT_PI / 180.0;
        EmbVector v = embVector_unit(angle);
        if (i%2 == 0) {
            v = v * distOuter;
        }
        else {
            v = v * distInner;
        }
        setRubberPoint(
            QString::fromStdString("POLYGON_POINT_" + std::to_string(i)),
            center.x + v.x,
            center.y + v.y);
    }
    setRubberText("POLYGON_NUM_POINTS", (properties["numPoints*2"].i - 1).toString());
}
#endif

