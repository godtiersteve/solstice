#include "bone_loader.h"
namespace solstice {
    BoneLoader::BoneLoader() {
        mCurrentScene = NULL;
        mCurrentModel = NULL;
    }
    void BoneLoader::LoadAnimatedModels(std::string dir, std::string prefix, std::string ext, TextureArray& textures) {
        dir = dir + "\\";
        std::string path = dir + prefix + "*" + ext;
        std::deque<std::string> files;
        WIN32_FIND_DATA fileData;
        HANDLE hFind;
        if(!((hFind = FindFirstFile(path.c_str(), &fileData)) == INVALID_HANDLE_VALUE)) {
            files.push_back(fileData.cFileName);
            while(FindNextFile(hFind, &fileData)) {
                files.push_back(fileData.cFileName);
            }
        }
        if(files.empty()) {
            return;
        }
        FindClose(hFind);
        for(size_t i = 0; i < files.size(); i++) {
            std::string file = dir + files[i];
            size_t start = file.find('_') + 1;
            size_t len = file.find('.') - start;
            std::string animName = file.substr(start, len);
            AddAnimation(file, prefix, animName);
        }
    }
    SkeletalModel BoneLoader::LoadModel(string file, TextureArray& textures, bool singleMesh, bool setup) {
        SkeletalModel m;
        Assimp::Importer importer;
        mCurrentScene = importer.ReadFile(file,
                                          aiProcess_CalcTangentSpace       |
                                          aiProcess_Triangulate            |
                                          aiProcess_JoinIdenticalVertices  |
                                          aiProcess_SortByPType);
        std::string path;
        size_t last = file.find_last_of("\\");
        if(last == string::npos) {
            last = file.find_last_of("/");
        }
        if(last != string::npos) {
            path = file.substr(0, last + 1);
        }
        if(!mCurrentScene) {
            std::cout << "Failed to read file: " << file << std::endl;
            return m;
        }
        textures.SetPath(path);
        mCurrentModel = &m;
        GenNodes(mCurrentScene->mRootNode);
        ProcessNode(mCurrentScene->mRootNode, textures, false);
        ModelAnimation basePose;
        basePose.GetBones() = mCurrentModel->GetBones();
        AnimKeyFrame f;
        f.bones = basePose.GetBones();
        basePose.AddFrame(f);
        f.timestamp = 1.0f;
        basePose.AddFrame(f);
        basePose.SetName("Base Pose");
        basePose.SetEndTime(1.0f);
        basePose.SetLoops(-1);
        //basePose.SetID(-1);
        textures.SetPath("");
        for(size_t k = 0; k < mCurrentScene->mNumAnimations; k++) {
            ModelAnimation anim;
            for(size_t i = 0; i < mCurrentScene->mAnimations[k]->mChannels[0]->mNumPositionKeys; i++) {
                LoadAnimation(anim, mCurrentScene->mRootNode, i);
            }
            anim.GetBones() = mCurrentModel->GetBones();
            anim.SetLoops(-1);
            anim.GenBones();
            if(anim.GetRef().empty()) {
                anim.SetName("Base Pose");
            }
            mCurrentModel->AddAnimation(anim);
        }
        if(mCurrentModel->GetAnimations().size() == 1) {
            //mCurrentModel->GetAnimation(0)->SetID(-1);
            //*mCurrentModel->GetAnimation(0) = basePose;
        }
        if(mCurrentModel->GetAnimations().empty()) {
            mCurrentModel->AddAnimation(basePose);
        }
        mCurrentModel->SortMeshes();
        if(setup) {
            mCurrentModel->SetupModel();
        }
        mCurrentModel = NULL;
        mCurrentScene = NULL;
        mModels.push_back(m);
        mNodes.clear();
        return m;
    }
    void BoneLoader::AddAnimation(std::string file, std::string model, std::string animName) {
        Assimp::Importer importer;
        mCurrentScene = importer.ReadFile(file,
                                          aiProcess_CalcTangentSpace       |
                                          aiProcess_Triangulate            |
                                          aiProcess_JoinIdenticalVertices  |
                                          aiProcess_SortByPType);
        mCurrentModel = GetModelPtr(model);
        if(!mCurrentModel) {
            std::cout << "Error loading animation: " << file << std::endl;
            return;
        }
        std::cout << "Loading animation: " << file << std::endl;
        for(size_t k = 0; k < mCurrentScene->mNumAnimations; k++) {
            ModelAnimation anim;
            for(size_t i = 0; i < mCurrentScene->mAnimations[0]->mChannels[0]->mNumPositionKeys; i++) {
                LoadAnimation(anim, mCurrentScene->mRootNode, i);
            }
            anim.GetBones() = mCurrentModel->GetBones();
            anim.SetName(animName);
            anim.SetLoops(-1);
            anim.GenBones();
            mCurrentModel->AddAnimation(anim);
        }
        mCurrentModel = NULL;
        mCurrentScene = NULL;
        mNodes.clear();
    }
    void BoneLoader::LoadAnimation(string file, string modelName) {
        mCurrentModel = GetModelPtr(modelName);
        if(!mCurrentModel) {
            return;
        }
    }
    void BoneLoader::GenNodes(aiNode* node) {
        mNodes.push_back(node);
        for(size_t i = 0; i < node->mNumChildren; i++) {
            GenNodes(node->mChildren[i]);
        }
    }
    void BoneLoader::ProcessNode(aiNode* node, TextureArray& textures, bool singleMesh) {
        for(size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = mCurrentScene->mMeshes[node->mMeshes[i]];
            SkeletalMesh sMesh;
            std::cout << "Loading mesh: " << mesh->mName.data << std::endl;
            ProcessMesh(mesh, sMesh, textures);
            sMesh.GetInverseTrans() = glm::inverse(glm::transpose(aiToGLMat4(mCurrentScene->mRootNode->mTransformation))) * aiToGLMat4(node->mTransformation);
            mCurrentModel->GetInverseTrans() = glm::inverse(glm::transpose(aiToGLMat4(mCurrentScene->mRootNode->mTransformation)));
            sMesh.SetName(mesh->mName.data);
            if(singleMesh) {
                mCurrentModel->AddVertices(sMesh);
                LoadBonesModel(mesh, sMesh);
            }
            else {
                LoadBonesModel(mesh, sMesh);
                mCurrentModel->AddMesh(sMesh);
            }
        }
        for(size_t i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], textures, singleMesh);
        }
    }
    void BoneLoader::ProcessMesh(const aiMesh* mesh, SkeletalMesh& m, TextureArray& textures) {
        aiString texRef;
        int texID = -1;
        if(mesh->mMaterialIndex >= 0) {
            aiMaterial* material = mCurrentScene->mMaterials[mesh->mMaterialIndex];
            material->Get(AI_MATKEY_NAME, texRef);
            for(size_t i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
                aiString str;
                material->GetTexture(aiTextureType_DIFFUSE, i, &str);
                texID = textures.LoadImg(str.C_Str(), texRef.C_Str());
            }
        }
        m.SetTextureID(texID);
        for(size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex3f vt;
            vt.position.x = mesh->mVertices[i].x;
            vt.position.y = mesh->mVertices[i].y;
            vt.position.z = mesh->mVertices[i].z;
            vt.normal.x = mesh->mNormals[i].x;
            vt.normal.y = mesh->mNormals[i].y;
            vt.normal.z = mesh->mNormals[i].z;
            if(mesh->HasVertexColors(i)) {
                vt.color.red = mesh->mColors[i]->r;
                vt.color.green = mesh->mColors[i]->g;
                vt.color.blue = mesh->mColors[i]->b;
                vt.color.alpha = mesh->mColors[i]->a;
            }
            if(mesh->mTextureCoords[0]) {
                vt.texCoord.x = mesh->mTextureCoords[0][i].x;
                vt.texCoord.y = mesh->mTextureCoords[0][i].y;
            }
            else {
                vt.texCoord.x = -1000.0f;
                vt.texCoord.y = -1000.0;
            }
            m.AddVertex(vt);
        }
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(size_t j = 0; j < face.mNumIndices; j++) {
                m.AddIndex(face.mIndices[j]);
            }
        }
    }
    void BoneLoader::LoadBonesModel(const aiMesh* mesh, SkeletalMesh& bmesh) {
        std::vector<Bone>& bones = mCurrentModel->GetBones();
        for(size_t i = 0; i < mesh->mNumBones; i++) {
            size_t boneIndex = 0;
            string nodeName = mesh->mBones[i]->mName.data;
            std::string boneName = mesh->mBones[i]->mName.data;
            std::string boneGroup;
            TrimNames(nodeName, boneName, boneGroup);
            auto mapID = mCurrentModel->GetBone(boneName);
            if(!mapID) {
                Bone b(boneName);
                b.offsetMatrix = aiToGLMat4(mesh->mBones[i]->mOffsetMatrix);
                mCurrentModel->AddDeformBone(b);
                mCurrentModel->AddBoneGroup(boneGroup, bones.size() - 1);
                aiNode* parentNode = GetNode(nodeName);
                if(parentNode->mParent) {
                    std::string parentName = parentNode->mParent->mName.data;
                    TrimNames(parentName, parentName, boneGroup);
                    bones.back().parentName = parentName;
                }
            }
            boneIndex = mCurrentModel->GetBoneID(boneName);
            for(size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
                size_t vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;
                Vertex3f* vt = &bmesh.GetVertex(vertexID);
                vt->AddWeight(boneIndex, weight);
            }
        }
    }
    aiNode* BoneLoader::GetNode(string str) {
        for(auto& node : mNodes) {
            if(node->mName.data == str) {
                return node;
            }
        }
        return NULL;
    }
    void BoneLoader::LoadAnimation(ModelAnimation& anim, const aiNode* node, size_t frame) {
        if(mCurrentScene->mNumAnimations == 0) {
            return;
        }
        const aiAnimation* animation = mCurrentScene->mAnimations[0];
        string nodeName = node->mName.data;
        const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);
        std::string groupName;
        std::string boneName = nodeName;
        TrimNames(nodeName, boneName, groupName);
        glm::vec3 scale(1), trans(0);
        glm::quat rot;
        if(frame >= anim.GetNumFrames()) {
            AnimKeyFrame f;
            f.bones = mCurrentModel->GetBones();
            anim.AddFrame(f);
        }
        AnimKeyFrame& keyFrame = anim[frame];
        if(nodeAnim) {
            aiVector3D scaleAi = nodeAnim->mScalingKeys[frame].mValue;
            scale = glm::vec3(scaleAi.x, scaleAi.y, scaleAi.z);
            aiVector3D transAi = nodeAnim->mPositionKeys[frame].mValue;
            trans = glm::vec3(transAi.x, transAi.y, transAi.z);
            aiQuaternion rotQuat = nodeAnim->mRotationKeys[frame].mValue;
            rot = glm::quat(rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z);
            //keyFrame.timestamp = (nodeAnim->mPositionKeys[frame].mTime / animation->mDuration);
            keyFrame.timestamp = nodeAnim->mPositionKeys[frame].mTime * animation->mTicksPerSecond;
            anim.SetEndTime(animation->mDuration * animation->mTicksPerSecond);
        }
        if(keyFrame.GetBone(boneName)) {
            Bone& bone = keyFrame.FindBone(boneName);
            bone.scale = scale;
            bone.translation = trans;
            bone.rotation = rot;
        }
        else if(nodeAnim) {
            Bone b(boneName);
            b.scale = scale;
            b.translation = trans;
            b.rotation = rot;
            if(node->mParent) {
                std::string parentName = node->mParent->mName.data;
                TrimNames(parentName, parentName, groupName);
                b.parentName = parentName;
            }
            mCurrentModel->AddBone(b);
            keyFrame.AddBone(b);
            //std::cout << "Extra bone: " << boneName << std::endl;
        }
        for(size_t i = 0; i < node->mNumChildren; i++) {
            LoadAnimation(anim, node->mChildren[i], frame);
        }
    }
    void BoneLoader::LoadAnimationModel(const aiNode* node, size_t frame) {
        /*
        if(mCurrentScene->mNumAnimations == 0) {
            return;
        }
        const aiAnimation* animation = mCurrentScene->mAnimations[0];
        string nodeName = node->mName.data;
        const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);
        glm::vec3 scale(1), trans(0);
        glm::quat rot;
        if(frame >= mCurrentModel->GetKeyFrames().size()) {
            AnimKeyFrame f;
            f.bones = mCurrentModel->GetBones();
            mCurrentModel->AddKeyFrame(f);
        }
        AnimKeyFrame& keyFrame = mCurrentModel->GetKeyFrame(frame);
        if(nodeAnim) {
            aiVector3D scaleAi = nodeAnim->mScalingKeys[frame].mValue;
            scale = glm::vec3(scaleAi.x, scaleAi.y, scaleAi.z);
            aiVector3D transAi = nodeAnim->mPositionKeys[frame].mValue;
            trans = glm::vec3(transAi.x, transAi.y, transAi.z);
            aiQuaternion rotQuat = nodeAnim->mRotationKeys[frame].mValue;
            rot = glm::quat(rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z);
            keyFrame.timestamp = (nodeAnim->mPositionKeys[frame].mTime / animation->mDuration) * animation->mTicksPerSecond;
        }
        if(keyFrame.GetBone(nodeName)) {
            Bone& bone = keyFrame.FindBone(nodeName);
            bone.scale = scale;
            bone.translation = trans;
            bone.rotation = rot;
        }
        else if(nodeAnim && node->mNumChildren) {
            Bone b(nodeName);
            b.scale = scale;
            b.translation = trans;
            b.rotation = rot;
            if(node->mParent) {
                b.parentName = node->mParent->mName.data;
            }
            mCurrentModel->AddBone(b);
            keyFrame.AddBone(b);
        }
        for(size_t i = 0; i < node->mNumChildren; i++) {
            LoadAnimationModel(node->mChildren[i], frame);
        }
        */
    }
    void BoneLoader::ExportModels(std::string filename) {
        std::ofstream out;
        for(auto& model : mModels) {
            //model.SaveToBinary(out);
        }
    }
    void BoneLoader::LoadBinary(std::string path) {
    }
    const aiNodeAnim* BoneLoader::FindNodeAnim(const aiAnimation* anim, string name) {
        for(size_t i = 0; i < anim->mNumChannels; i++) {
            const aiNodeAnim* nodeAnim = anim->mChannels[i];
            if(nodeAnim->mNodeName.C_Str() == name) {
                return nodeAnim;
            }
        }
        return NULL;
    }
    aiBone* BoneLoader::GetBone(const aiNode* node, string str) {
        for(size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = mCurrentScene->mMeshes[node->mMeshes[i]];
            for(size_t j = 0; j < mesh->mNumBones; j++) {
                aiBone* bone = mesh->mBones[j];
                if(bone->mName.C_Str() == str) {
                    return bone;
                }
            }
        }
        return NULL;
    }

    void BoneLoader::clear() {
        if(mCurrentModel) {
            mCurrentModel->clear();
        }
        mCurrentModel = NULL;
        mCurrentScene = NULL;
    }
    SkeletalModel* BoneLoader::GetModelPtr(std::string n) {
        for(size_t i = 0; i < mModels.size(); i++) {
            if(mModels[i].GetName() == n) {
                return &mModels[i];
            }
        }
        return NULL;
    }
    void TrimNames(std::string str, std::string& name, std::string& group) {
        size_t startSize = str.find('_');
        if(startSize == std::string::npos) {
            return;
        }
        std::string sub = str.substr(startSize + 1, str.size() - startSize + 1);
        size_t endSize = sub.find('_');
        if(endSize == std::string::npos) {
            return;
        }
        group = sub.substr(0, endSize);
        //printf("Group: %s\n", group.c_str());
        size_t size = str.size() - startSize + 2 + group.size();
        name = str.substr(startSize + 2 + group.size(), size);
    }
    bool LoadModelAndAnimations(SkeletalModel& model, std::string path, std::string prefix, TextureArray& textures) {
        BoneLoader loader;
        loader.LoadModel(path + "\\meshes\\" + prefix + ".dae", textures, false, false);
        if(loader.GetModels().empty()) {
            return false;
        }
        else {
            loader.GetModel(0).SetName(prefix);
            loader.LoadAnimatedModels(path + "\\animations\\", prefix, ".dae", textures);
            model = loader.GetModel(0);
            model.SetupModel();
            return true;
        }
    }
}
