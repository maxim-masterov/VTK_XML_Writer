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

#ifndef XMLWRITER_INL_
#define XMLWRITER_INL_

namespace xmlw {

inline bool VTK_XML_Writer::IsLittleEndian() {
    short int number = 0x1;
    char *numPtr = (char*)&number;
    return (numPtr[0] == 1);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenSection(std::string str, Stream &stream) {
    str = indentation + "<" + str + ">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::OneLineSection(std::string str, Stream &stream) {
    str = indentation + "<" + str + "/>\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::CloseSection(std::string str, Stream &stream) {
    //indentation.erase(indentation.length() - 2);
    str = indentation + "</" + str + ">\n";
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::OpenDataArrSection(const std::string type, const std::string name,
        const size_t num_of_comp, const std::string format, const size_t offset,
        Stream &stream) {

    std::string str;
    str = indentation + "<DataArray type=\"" + type + "\" Name=\"" + name
            + "\" NumberOfComponents=\"" + std::to_string(num_of_comp) + "\" format=\"" + format
            + "\" offset=\"" + std::to_string(offset) + "\"" + ">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::OpenPDataArrSection(const std::string type, const std::string name,
        const size_t num_of_comp, const std::string format, const size_t offset,
        Stream &stream) {

    std::string str;
    str = indentation + "<PDataArray type=\"" + type + "\" Name=\"" + name
            + "\" NumberOfComponents=\"" + std::to_string(num_of_comp) + "\" format=\"" + format
            + "\" offset=\"" + std::to_string(offset) + "\"" + ">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::ClosePDataArrSection(Stream &stream) {
    CloseSection("PDataArray", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenDataArrSection(const std::string type, const std::string name,
        const size_t num_of_comp, const std::string format, Stream &stream) {

    std::string str;
    str = indentation + "<DataArray type=\"" + type + "\" Name=\"" + name
            + "\" NumberOfComponents=\"" + std::to_string(num_of_comp) + "\" format=\"" + format
            + "\"" + ">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::CloseDataArrSection(Stream &stream) {
    CloseSection("DataArray", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenVTKSection(const std::string type, Stream &stream) {
    std::string str;
    str = indentation + "<VTKFile type=\"" + type + "\" version=\"" + vtk_version
            + "\" byte_order=\"" + byte_order + "\">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::CloseVTKSection(Stream &stream) {
    CloseSection("VTKFile", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenPieceSection(const size_t num_points, const size_t num_cells, Stream &stream) {
    std::string str;
    str = indentation + "<Piece NumberOfPoints=\"" + std::to_string(num_points)
            + "\" NumberOfCells=\"" + std::to_string(num_cells) + "\">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::ClosePieceSection(Stream &stream) {
    CloseSection("Piece", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenPointDataSection(const std::string name, Stream &stream) {
    std::string str;
    str = indentation + "<PointData Scalars=\"" + name + "\">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::ClosePointDataSection(Stream &stream) {
    CloseSection("PointData", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenPPointDataSection(const std::string name, Stream &stream) {
    std::string str;
    str = indentation + "<PPointData Scalars=\"" + name + "\">\n";
    //indentation.append(2, ' ');
    stream << str;
}

template<typename Stream>
inline void VTK_XML_Writer::ClosePPointDataSection(Stream &stream) {
    CloseSection("PPointData", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::OpenCoordinatesSection(Stream &stream) {
    OpenSection("Coordinates", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::CloseCoordinatesSection(Stream &stream) {
    CloseSection("Coordinates", stream);
}

template<typename Stream>
inline void VTK_XML_Writer::Header(Stream &stream) {
    stream << "<?xml version=\"" + xml_version + "\"?>\n";
}

template<typename Data, typename Stream>
inline size_t VTK_XML_Writer::WriteData(Data &data, Stream &stream) {
    const size_t size = data.size();
    for(size_t n = 0; n < size; ++n)
        stream << data[n] << " ";
    stream << "\n";
    return CountOffset(data);
}

template<typename Data, typename Stream>
inline void VTK_XML_Writer::AppendData(Data &data, Stream &stream) {
    uint32_t size = sizeof(data[0]) * data.size();
    stream.write((char*)&size, sizeof(uint32_t));
    stream.write((char*)data.data(), size);
}

template<typename Data>
inline size_t VTK_XML_Writer::CountOffset(Data &data) {
    const size_t size = data.size();
    return sizeof(data.at(0)) * size;
}

template<typename Data>
inline size_t VTK_XML_Writer::CountOffsetGrid(Data &data) {
    const size_t size = data.size();
    return sizeof(data.at(0).x) * size;
}

inline void VTK_XML_Writer::SetXMLVersion(const std::string _version) {
    xml_version = _version;
}

inline void VTK_XML_Writer::SetVTKVersion(const std::string _version) {
    vtk_version = _version;
}

template <typename T>
inline std::string VTK_XML_Writer::CheckDataType(const T data) {

    // Types supported by VTK
    //    Int8, UInt8, Int16, UInt16, Int32,
    //    UInt32, Int64, UInt64, Float32, Float64

    std::string type;

    if ( typeid(data).name() == typeid(float).name() ) {
        type = "Float32";
    }
    if ( typeid(data).name() == typeid(double).name() ) {
        type = "Float64";
    }
    if ( typeid(data).name() == typeid(int).name() ) {
        type = "Int32";
    }
    if ( typeid(data).name() == typeid(uint8_t).name() ) {
        type = "UInt8";
    }
    if ( typeid(data).name() == typeid(uint16_t).name() ) {
        type = "UInt16";
    }
    if ( typeid(data).name() == typeid(uint32_t).name() ) {
        type = "UInt32";
    }
    if ( typeid(data).name() == typeid(size_t).name() ) {
        if (sizeof(size_t) == 8)
            type = "UInt64";
        if (sizeof(size_t) == 4)
            type = "UInt32";
    }
    if ( typeid(data).name() == typeid(long).name() ) {
        if (sizeof(long) == 8)
            type = "UInt64";
        if (sizeof(long) == 4)
            type = "UInt32";
    }

    if (type.empty())
        std::cerr << "Error! Unknown data type : " << typeid(data).name() << " of size of "
                << sizeof(data) << " Bytes. See " << __FILE__ << ":" << __LINE__ << "\n";

    return type;
}

}



#endif /* XMLWRITER_INL_ */
