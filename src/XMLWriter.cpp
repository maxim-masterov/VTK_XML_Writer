/************************************************************************************
 *                                                                                  *
 * Copyright 2018 Maxim Masterov                                                    *
 *                                                                                  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy     *
 * of this software and associated documentation files (the "Software"), to deal    *
 * in the Software without restriction, including without limitation the rights     *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
 * copies of the Software, and to permit persons to whom the Software is            *
 * furnished to do so, subject to the following conditions:                         *
 *                                                                                  *
 * The above copyright notice and this permission notice shall be included in       *
 * all copies or substantial portions of the Software.                              *
 *                                                                                  *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS          *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING          *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS     *
 * IN THE SOFTWARE.                                                                 *
 *                                                                                  *
 ************************************************************************************/

#include "XMLWriter.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <FancyBear.h>

namespace xmlw {

class Coord {
public:
    float x;
    float y;
    float z;

    friend inline std::ostream &operator<< (std::ostream& os, const Coord &v) {
        os << v.x << ' ' << v.y << ' ' << v.z;
        return os;
    }
};

void VTK_XML_Writer::TestUntructuredOutput() {

    std::ofstream os;
    std::ostringstream ostr;
    xmlw::VTK_XML_Writer wxml;
    std::vector<float> data1, data2;
    std::vector<float> points;
    std::vector<int> cells;
    std::vector<int> offsets;
    std::vector<uint8_t> types;

    data1.resize(27);
    data2.resize(27);
    points.resize(81);
    cells.resize(64);
    offsets.resize(8);
    types.resize(8);

    data1 = { -0.4, -0.4, -0.4, -0.4, -1, -0.4, -0.4, -0.4, -0.4, 0.4, 1, 0.4, 1,
            0.4, 1, 0.4, 1, 0.4, 0.4, 0.4, 0.4, 0.4, 1, 0.4, 0.4, 0.4, 0.4 };

    data2 = { -13.0, -12.0, -11.0, -10.0, -9.0, -8.0, -7.0, -6.0, -5.0, -4.0, -3.0, -2.0,
            -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0};

    points = {
        0, 0, 0, 0, 0, 1, 0, 0, 2,
        0, 1, 0, 0, 1, 1, 0, 1, 2,
        0, 2, 0, 0, 2, 1, 0, 2, 2,
        1, 0, 0, 1, 0, 1, 1, 0, 2,
        1, 1, 0, 1, 1, 1, 1, 1, 2,
        1, 2, 0, 1, 2, 1, 1, 2, 2,
        2, 0, 0, 2, 0, 1, 2, 0, 2,
        2, 1, 0, 2, 1, 1, 2, 1, 2,
        2, 2, 0, 2, 2, 1, 2, 2, 2
    };

    cells = {
        0,  9,  3, 12, 1, 10, 4, 13,
        1,  10, 4, 13, 2, 11, 5, 14,
        3,  12, 6, 15, 4, 13, 7, 16,
        4,  13, 7, 16, 5, 14, 8, 17,
        9,  18, 12, 21, 10, 19, 13, 22,
        10, 19, 13, 22, 11, 20, 14, 23,
        12, 21, 15, 24, 13, 22, 16, 25,
        13, 22, 16, 25, 14, 23, 17, 26
    };

    offsets = { 8, 16, 24, 32, 40, 48, 56, 64 };
    types = { 11, 11, 11, 11, 11, 11, 11, 11 };

    os.open("test.vtu", std::ios::out | std::ios::binary);
    std::string format = "appended";
    size_t bofs = 0;
    size_t size_of_dt = 4;

    /* Assemble the body */
    wxml.Header(ostr);
    wxml.OpenVTKSection("UnstructuredGrid", ostr);
        wxml.OpenSection("UnstructuredGrid", ostr);
            wxml.OpenPieceSection(27, 8, ostr);

                /* Should be appended to the file */
                wxml.OpenPointDataSection("scalars", ostr);
                    wxml.OpenDataArrSection("Float32", "scalars", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(data1) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);

                    wxml.OpenDataArrSection("Float32", "mytest", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(data2) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);
                wxml.ClosePointDataSection(ostr);

                wxml.OpenSection("Points", ostr);
                    wxml.OpenDataArrSection("Float32", "points", 3, format, bofs, ostr);
                    bofs += wxml.CountOffset(points) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);
                wxml.CloseSection("Points", ostr);

                wxml.OpenSection("Cells", ostr);
                    wxml.OpenDataArrSection("Int32", "connectivity", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(cells) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);

                    wxml.OpenDataArrSection("Int32", "offsets", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(offsets) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);

                    wxml.OpenDataArrSection("UInt8", "types", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(types) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);
                wxml.CloseSection("Cells", ostr);
                /* ****************************** */

            wxml.ClosePieceSection(ostr);
        wxml.CloseSection("UnstructuredGrid", ostr);

        wxml.OpenSection("AppendedData encoding=\"raw\"", ostr);


    os << ostr.str();
    os << "_";
    wxml.AppendData(data1, os);
    wxml.AppendData(data2, os);
    wxml.AppendData(points, os);
    wxml.AppendData(cells, os);
    wxml.AppendData(offsets, os);
    wxml.AppendData(types, os);

    wxml.CloseSection("AppendedData", os);
    wxml.CloseVTKSection(os);

    os.close();
//    <DataArray ... format="appended" offset="0"/>
//    The value in the "offset" attribute is the file offset in bytes beyond the leading underscore. An offset of "0" means the first character after the underscore. Each DataArray's data are stored in a contiguous block. The block can be either compressed or uncompressed, but by default it is uncompressed (compression is global to the file and marked by an attribute on the VTKFile element).
}

void VTK_XML_Writer::TestStructuredOutput() {

    std::ofstream os;
    std::ostringstream ostr;
    xmlw::VTK_XML_Writer wxml;
    fb::variables<Coord> grid;
    std::vector<float> data1, data2;
    int NI = 3;
    int NJ = 3;
    int NK = 1;
    double L = 1.;
    double H = 1.;
    double D = 1.;
    double dx = L / (NI-1);
    double dy = H / (NJ-1);
    double dz = D / (NK-1);

    grid.allocate(NI,  NJ,  NK);
    data1.resize(grid.size());
    data2.resize(grid.size());
    int n = 0;
    for(fb::Index_t i = 0; i < grid.NI_init(); ++i) {
        for(fb::Index_t j = 0; j < grid.NJ_init(); ++j) {
            for(fb::Index_t k = 0; k < grid.NK_init(); ++k) {
                grid(i, j, k).x = i * dx;
                grid(i, j, k).y = j * dy;
                grid(i, j, k).z = k * dz;
                data1[n] = i + j + k;
                data2[n] = i * j * k;
                ++n;
            }
        }
    }

    os.open("test.vts", std::ios::out);// | std::ios::binary);
    std::string format = "appended";
    size_t bofs = 0;
    size_t size_of_dt = 4;
    std::string extent = "\"1 " + std::to_string(NI) + " 1 " + std::to_string(NJ) + " 1 " + std::to_string(NK) + "\"";

    /*
     * Note, counting starts from 1 (not from 0!)
     */

    /* Assemble the body */
    wxml.Header(ostr);
    wxml.OpenVTKSection("StructuredGrid", ostr);
        wxml.OpenSection("StructuredGrid WholeExtent=" + extent, ostr);
            wxml.OpenSection("Piece Extent=" + extent, ostr);

                /* Should be appended to the file */
                wxml.OpenSection("Points", ostr);
                    wxml.OpenDataArrSection("Float32", "coord", 3, format, bofs, ostr);
                    bofs += wxml.CountOffset(grid) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);
                wxml.CloseSection("Points", ostr);

                wxml.OpenPointDataSection("scalars", ostr);
                    wxml.OpenDataArrSection("Float32", "scalars", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(data1) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);

                    wxml.OpenDataArrSection("Float32", "mytest", 1, format, bofs, ostr);
                    bofs += wxml.CountOffset(data2) + size_of_dt;
                    wxml.CloseDataArrSection(ostr);
                wxml.ClosePointDataSection(ostr);
                /* ****************************** */

            wxml.CloseSection("Piece", ostr);
        wxml.CloseSection("StructuredGrid", ostr);

    os << ostr.str();
    wxml.OpenSection("AppendedData encoding=\"raw\"", os);
    os << "_";
    wxml.AppendData(grid, os);
    wxml.AppendData(data1, os);
    wxml.AppendData(data2, os);
    wxml.CloseSection("AppendedData", os);
    wxml.CloseVTKSection(os);

    os.close();
}

}


