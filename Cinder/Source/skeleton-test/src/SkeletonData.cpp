#include "SkeletonData.h"


SkeletonData::SkeletonData(void)
{
}

void SkeletonData::SaveSkeleton(string directory, string fileName, vector<Joint *> &m_joints) {
	try{
		//Validate directory
		validateDirectory(directory);

		//Validate filename
		validateFileName(fileName);

		//Create dir
		if( PathFileExistsA(directory.c_str()) == TRUE)  { 
			app::console() << "dir created/exists" << endl;
			ofstream boneFile;
			try {
				boneFile.open(directory.append(fileName));
				std::stringstream json = std::stringstream();

				boneFile << "{\"bones\":[" << endl;
				for(Joint * b : m_joints)
				{ 
					boneFile << boneJson(b);
					if (b->id != m_joints.back()->id) {
						boneFile << ",";
					}
					boneFile << endl;
				}
				boneFile << "]}" << endl;
				boneFile.close();
			}catch (exception ex){
				if(boneFile != NULL){
					if (boneFile.is_open()){
						boneFile.close();
					}
				}
				throw ex;
			}
		}else{
				app::console() << "Does Not exist!\n" << endl;
				throw exception("Directory does not exist!");
		}
	}catch(exception ex){
		throw ex;
	}
}

string SkeletonData::boneJson(Joint* b) {
	std::stringstream json;
	json << " {" << endl;
	json << "  \"id\":" << b->id << "," << endl;
	if (b->parent != NULL) {
		json << "  \"parent_id\":" << b->parent->id << "," << endl;
	}
	json << "  \"pos\":"<< "{\"x\":" << b->getPos().x << ", \"y\":" << b->getPos().y << ", \"z\":" << b->getPos().z << "}," << endl;
	json << "  \"orientation\":" << "{\"x\":" << b->orientation.v.x << ", \"y\":" << b->orientation.v.y << ", \"z\":" << b->orientation.v.z << ", \"w\":" << b->orientation.w << "}," << endl;
	json << "  \"children\":" << "[" << endl;
	for(Joint * c : b->children) {
		json << boneJson(c);
		if (c->id != c->children.back()->id) {
			json << ",";
		}
	}
	
	json << "]" << endl;
	json << " }" << endl;

	return json.str();
}

vector<Joint*> SkeletonData::LoadSkeleton(string filePath) {
	
	
	vector<Joint*> joints;
	ifstream boneFile(filePath);

	if (boneFile.good())
	{
		static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
		
			
		FILE* pFile = fopen(filePath.c_str(), "rb");
		char buffer[65536];
		FileReadStream is(pFile, buffer, sizeof(buffer));
		Document document;

		if (document.ParseStream<0, UTF8<>, FileReadStream>(is).HasParseError() == true) {
			app::console() << "Parsing error" << endl;
		}else{
			for(Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
			app::console() << "Type of member " << itr->name.GetString() << " is " << kTypeNames[itr->value.GetType()] << endl;

			Value& bones = document["bones"];
			Joint * parent;

			for(rapidjson::Value::ConstMemberIterator it=bones.MemberBegin(); it != bones.MemberEnd(); it++) {
				Joint * j;
				if (parent != NULL){
					j = new Joint(parent);
				}else{
					j = new Joint();
				}

				j->id = it->value["id"].GetInt64(); //Don't know what to use for long
				j->setPos(Vec3f(it->value["pos"]["x"].GetInt(),it->value["pos"]["y"].GetInt(),it->value["pos"]["z"].GetInt()));
				j->orientation = Quatd(it->value["orientation"]["x"].GetInt(),it->value["orientation"]["y"].GetInt(),it->value["orientation"]["z"].GetInt(),it->value["orientation"]["w"].GetInt());

				joints.push_back(j);
				parent = j;
			}
		}
	}else{
		//throw exception("File does not exist.");
		app::console() << "File does not exist!" << endl;
	}
	return joints;
}

vector<Joint*> SkeletonData::LoadSkeletonJson(string filePath) {
	vector<Joint*> joints;

	if( PathFileExistsA(filePath.c_str()) == TRUE)  { 
		try{
			JsonTree doc = JsonTree(loadFile(filePath));

			JsonTree bones = doc.getChild( "bones" );
			Joint * parent = NULL;
			for( JsonTree::ConstIter bone = bones.begin(); bone != bones.end(); ++bone ) {
				Joint * j;
				if (parent != NULL){
					j = new Joint(parent);
				}else{
					j = new Joint();
				}
					
				j->id = bone->getChild( "id" ).getValue<int>();

				JsonTree pos = bone->getChild("pos");
				j->setPos(Vec3f(pos.getChild("x").getValue<float>(),pos.getChild("y").getValue<float>(),pos.getChild("z").getValue<float>()));

				JsonTree orientation = bone->getChild("orientation");
				j->orientation = Quatd(orientation.getChild("x").getValue<float>(),orientation.getChild("y").getValue<float>(),orientation.getChild("z").getValue<float>(),orientation.getChild("w").getValue<float>());

				joints.push_back(j);

				parent = j;
				/*app::console() << "id: " << bone->getChild( "id" ).getValue<int>() << endl;
						
				app::console() << "pos: x: " << bone->getChild("pos.x").getValue<float>() << " y: " << bone->getChild("pos.y").getValue<float>() << " z: " << bone->getChild("pos.z").getValue<float>() << endl;
				app::console() << "orientation: x: " << bone->getChild("orientation.x").getValue<float>() << " y: " << bone->getChild("orientation.y").getValue<float>() << " z: " << bone->getChild("orientation.z").getValue<float>() << " w: " << bone->getChild("orientation.w").getValue<float>() << endl;*/
						
				/*if (bone->hasChild("parent_id")) {

					app::console() << "parent_id: " << bone->getChild( "parent_id" ).getValue<int>() << endl;
				}*/		
			}
		}catch (exception ex) {
			//dunno for now
		}
	}
	return joints;
	//throw(errno);
}

void SkeletonData::validateDirectory(string &directory) {
	if( PathFileExistsA(directory.c_str()) == TRUE)  { 
		if ( directory.back() != (char)"/") {
			directory += "/";
		}
		//this->directory = directory;
		// directory ok!
	}else{
		app::console() << "Directory does not exist!" << endl;
		throw exception("Directory does not exist!");
	}
}

void SkeletonData::validateFileName(string &fileName) {
	regex rgx_name("[\\\\/<>\|\":\?\*]+");
	regex rgx_ext("^.*\\.json$");
	try{
		app::console() << "validateFileName" << endl;
		if(fileName.empty()){
			app::console() << "Filename is empty" << endl;
			throw exception("Filename is empty");
		}
		if(regex_match(fileName.begin(),fileName.end(),rgx_name)){
			app::console() << "Invalid filename" << endl;
			throw exception("Invalid filename: filename cannot contain \\/<>|\":?*");
		}
		if(!regex_match(fileName.begin(),fileName.end(),rgx_ext)){
			app::console() << "Missing extension" << endl;
			fileName.append(".json");
		}
		//this->fileName = fileName;
		// fileName ok!
	}catch(exception ex){
		throw ex;
	}
}

SkeletonData::~SkeletonData(void)
{
}
