#include "ObjectClass.h"

ObjectClass::ObjectClass() {
	position.x = position.y = position.z = 0.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

int ObjectClass::Initial(char *file)
{
	using jsonxx::Object;
	using jsonxx::Array;
	using jsonxx::Number;

	ifstream imported_file;
	imported_file.open(file, std::ios::in);
	if (!imported_file.is_open()) {
		return ERROR;
	}
	//delete this later!!!
	Object *tmpParserResult = new Object;

	tmpParserResult->parse(imported_file);
	imported_file.close();

	int vertice_count = tmpParserResult->get<Array>("vertices").size();
	for (int lop = 0; lop < vertice_count; lop += 3) {
		vertices.push_back(
			Vector3(
				tmpParserResult->get<Array>("vertices").get<Number>(lop),
				tmpParserResult->get<Array>("vertices").get<Number>(lop + 1),
				tmpParserResult->get<Array>("vertices").get<Number>(lop + 2))
			);
	}

	int indice_count = tmpParserResult->get<Array>("indices").size();
	for (int lop = 0; lop < indice_count; lop++) {
		indices.push_back((int)(tmpParserResult->get<Array>("indices").get<Number>(lop)));
	}

	delete tmpParserResult;
	tmpParserResult = nullptr;

	return OK;
}

ObjectClass::ObjectClass(Vector3 _position) {
	position = _position;
	rotation.x = rotation.y = rotation.z = 0.0f;
}

ObjectClass::ObjectClass(Vector3 _position, Vector3 _rotation) {
	position = _position;
	rotation = _rotation;
}