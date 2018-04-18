//============================================================================
// Name        : XML_writer.cpp
// Author      : Maxim Masterov
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "XMLWriter.h"

using namespace std;

int main() {

    xmlw::VTK_XML_Writer my_xml;

    my_xml.TestStructuredOutput();
    // or
//    my_xml.TestUnstructuredOutput();

	return 0;
}


