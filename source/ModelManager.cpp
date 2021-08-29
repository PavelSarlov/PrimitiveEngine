#include "ModelManager.h"
#include <ErrorLogger.h>
#include <Model.h>

ModelManager::ModelManager()
{}

ModelManager::~ModelManager()
{}

ModelPtr ModelManager::createModelFromFile(const wchar_t *file_path)
{
	return std::static_pointer_cast<Model>(createResourceFromFile(file_path));
}

Resource *ModelManager::createResourceFromFileConcrete(const wchar_t *file_path)
{
	try
	{
		return new Model(file_path);
	}
	catch(std::exception e)
	{
		ErrorLogger::log(e.what());
	}

	return nullptr;
}
