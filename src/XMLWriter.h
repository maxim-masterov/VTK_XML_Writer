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

#ifndef XMLWRITER_H_
#define XMLWRITER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>

namespace xmlw {

/*!
 * \class VTK_XML_Writer
 * \brief XML writer for VTK files
 * One can consider two ways of writing XML files for VTK readers (like ParaView). First one is the simplest one
 * and consists of writing all data section by section in ASCII mode (the binary one requires Base64 encoding and is
 * a bit complicated). Using this way one has to perform next steps: open 'DataArray', write all data in, close
 * 'DataArray'. This method suffers from two major problems. First of all, ASCII files are usually large (if accuracy
 * is preserved). Second, it is quite hard to append data into the file (doable, but simply inconvenient), since one has
 * to preserve hierarchy of all already opened and closed sections. Another way to write XML files readable by VTK-based
 * software is to create a main body of the file, which will contain only info on data structures but will not contain
 * any data points and append data in a raw binary format to the very end of the file. This method leads to smaller file
 * size and also much more flexible since the main body of the XML file is written down and doesn't require any changes.
 * Despite the fact that most information of XML and VTK file structures can be found online (read it first), there are
 * few "things" which are mentioned nowhere (almost) with respect to data organization in appended files:
 *  - After opening an appended data section one has to put underscore symbol '_' (in ASCII mode) before writing any data
 *  - Each data set should start with 4-byte unsigned integer which represents the length of data set in bytes. This integer
 *    should be written in binary.
 *  - No spaces or any other characters are allowed between data sets, so the overall information can be read as a contiguous
 *    chunk
 * Thus, the appended data should look like:
 *   <AppendedData encoding="raw">
 *   _NNNNDDDDDDDD.....NNNNDDDDDDDDD......NNNNDDDDDDDDDDD....
 *   </AppendedData>
 * where NNNN are 4-byte integers and D - represents a data. Note also, that in a case of appended data one has to take into
 * account presence of these additional 4 bytes when calculating offsets for 'DataArray' sections (for more information see
 * documentation of VTK file formats)
 */
class VTK_XML_Writer {
public:

    /*!
     * \brief Default constructor
     */
    VTK_XML_Writer() {

        xml_version = "1.0";
        vtk_version = "0.1";

        indentation = "";

        if (IsLittleEndian())
            byte_order = "LittleEndian";
        else
            byte_order = "BigEndian";
    }

    /*!
     * \brief Deafult Destructor
     */
    virtual ~VTK_XML_Writer() { }

    /*!
     * \brief Writes down file with a structured grid in raw binary mode
     */
    void TestStructuredOutput();

    /*!
     * \brief Writes down file with a structured grid in raw binary mode
     */
    void TestUntructuredOutput();

    /*!
     * \brief Returns string of a data type for VTK format
     * \warning One should provide one raw value, not a container!
     */
    template <typename T>
    std::string CheckDataType(const T data);

    /*!
     * \brief Opens the section
     * @param str Name of the section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenSection(std::string str, Stream &stream);

    /*!
     * \brief Opens and closes the section in one line
     * @param str String to be written
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OneLineSection(std::string str, Stream &stream);

    /*!
     * \brief Closes the section
     * @param str Name of the section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void CloseSection(std::string str, Stream &stream);

    /*!
     * \brief Opens 'DataArray' section for binary output
     * @param type Data type (Int32, Float32, ...)
     * @param name The name of the data set
     * @param num_of_comp Number of components in each element of data
     * @param format Format of data (ascii, binary, appended)
     * @param offset Offset for appended data
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenDataArrSection(const std::string type, const std::string name,
            const size_t num_of_comp, const std::string format, const size_t offset,
            Stream &stream);

    /*!
     * \brief Opens 'PDataArray' section for binary output
     * @param type Data type (Int32, Float32, ...)
     * @param name The name of the data set
     * @param num_of_comp Number of components in each element of data
     * @param format Format of data (ascii, binary, appended)
     * @param offset Offset for appended data
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenPDataArrSection(const std::string type, const std::string name,
            const size_t num_of_comp, const std::string format, const size_t offset,
            Stream &stream);

    /*!
     * \brief Closes 'DataArray' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void ClosePDataArrSection(Stream &stream);

    /*!
     * \brief Opens 'DataArray' section for ASCII ouput
     * @param type Data type (Int32, Float32, ...)
     * @param name The name of the data set
     * @param num_of_comp Number of components in each element of data
     * @param format Format of data (ascii, binary, appended)
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenDataArrSection(const std::string type, const std::string name,
            const size_t num_of_comp, const std::string format, Stream &stream);

    /*!
     * \brief Closes 'DataArray' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void CloseDataArrSection(Stream &stream);

    /*!
     * \brief Opens 'VTK' section
     * @param type Type of the data
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenVTKSection(const std::string type, Stream &stream);

    /*!
     * \brief Closes 'VTK' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void CloseVTKSection(Stream &stream);

    /*!
     * \brief Opens 'Piece' section
     * @param num_points Number of points
     * @param num_cells Number of cells
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenPieceSection(const size_t num_points, const size_t num_cells, Stream &stream);

    /*!
     * \brief Closes 'Piece' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void ClosePieceSection(Stream &stream);

    /*!
     * \brief Opens 'PointData' section
     * @param name The name of the data set
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenPointDataSection(const std::string name, Stream &stream);

    /*!
     * \brief Closes 'PointData' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void ClosePointDataSection(Stream &stream);

    /*!
     * \brief Opens 'PPointData' section
     * @param name The name of the data set
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenPPointDataSection(const std::string name, Stream &stream);

    /*!
     * \brief Closes 'PPointData' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void ClosePPointDataSection(Stream &stream);

    /*!
     * \brief Opens 'Coordinates' section
     * @param name The name of the data set
     * @param stream Output stream
     */
    template<typename Stream>
    inline void OpenCoordinatesSection(Stream &stream);

    /*!
     * \brief Closes 'Coordinates' section
     * @param stream Output stream
     */
    template<typename Stream>
    inline void CloseCoordinatesSection(Stream &stream);

    /*!
     * \brief Writes the header of the XML file
     * @param stream Output stream
     */
    template<typename Stream>
    inline void Header(Stream &stream);

    /*!
     * \brief Writes data into the stream
     * \note Class Data should be compatible with STL library
     * @param data Reference to the data set
     * @param stream Reference to the output stream
     * @param stream Output stream
     */
    template<typename Data, typename Stream>
    inline size_t WriteData(Data &data, Stream &stream);

    /*!
     * \brief Appends data to the end of the file in a raw binary mode
     * \note Doesn't put any closing statements
     * @param data Reference to the data set
     * @param stream Reference to the output stream
     * @param stream Output stream
     */
    template<typename Data, typename Stream>
    inline void AppendData(Data &data, Stream &stream);

    /*!
     * \brief Counts size of the data set in Bytes
     * \note Class Data should be compatible with STL library.
     * \warning Class Data should return correct size of the internal data in Bytes when
     * sizeof() is using. Otherwise, this method will return the wrong offset, which may
     * lead to violation of data structure.
     * @param data Reference to the data set
     */
    template<typename Data>
    inline size_t CountOffset(Data &data);

    /*!
     * \brief Counts size of the grid in Bytes
     * This method is a safe form of the CountOffset() one, since sizeof() for the grid structure
     * may return wrong values sometimes (depends of a structure used to represent coordinates).
     * \note Class Data should be compatible with STL library and contain a structure with .x member.
     * @param data Reference to the data set
     */
    template<typename Data>
    inline size_t CountOffsetGrid(Data &data);

    /*!
     * \brief Sets version of XML format
     * @param _version Version to be set
     */
    inline void SetXMLVersion(const std::string _version);

    /*!
     * \brief Sets version of VTK format
     * @param _version Version to be set
     */
    inline void SetVTKVersion(const std::string _version);

private:
    /*!
     * \brief Returns true if system has little-endian byte order (false otherwise)
     */
    inline bool IsLittleEndian();

private:
    std::string indentation;
    std::string xml_version;
    std::string vtk_version;
    std::string byte_order;
};

} /* namespace xmlw */

#include "XMLWriter.inl"

#endif /* XMLWRITER_H_ */
