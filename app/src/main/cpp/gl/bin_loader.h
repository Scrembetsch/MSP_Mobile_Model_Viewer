#ifndef MSP_MOBILE_MODEL_VIEWER_BIN_LOADER_H
#define MSP_MOBILE_MODEL_VIEWER_BIN_LOADER_H

#include <cstring>

namespace binl
{
    // Structure: Vector2
    //
    // Description: A 2D Vector that Holds Positional Data
    struct Vector2
    {
        // Default Constructor
        Vector2()
        {
            X = 0.0f;
            Y = 0.0f;
        }
        // Variable Set Constructor
        Vector2(float X_, float Y_)
        {
            X = X_;
            Y = Y_;
        }
        // Bool Equals Operator Overload
        bool operator==(const Vector2& other) const
        {
            return (this->X == other.X && this->Y == other.Y);
        }
        // Bool Not Equals Operator Overload
        bool operator!=(const Vector2& other) const
        {
            return !(this->X == other.X && this->Y == other.Y);
        }
        // Addition Operator Overload
        Vector2 operator+(const Vector2& right) const
        {
            return Vector2(this->X + right.X, this->Y + right.Y);
        }
        // Subtraction Operator Overload
        Vector2 operator-(const Vector2& right) const
        {
            return Vector2(this->X - right.X, this->Y - right.Y);
        }
        // Float Multiplication Operator Overload
        Vector2 operator*(const float& other) const
        {
            return Vector2(this->X *other, this->Y * other);
        }

        // Positional Variables
        float X;
        float Y;
    };

    // Structure: Vector3
    //
    // Description: A 3D Vector that Holds Positional Data
    struct Vector3
    {
        // Default Constructor
        Vector3()
        {
            X = 0.0f;
            Y = 0.0f;
            Z = 0.0f;
        }
        // Variable Set Constructor
        Vector3(float X_, float Y_, float Z_)
        {
            X = X_;
            Y = Y_;
            Z = Z_;
        }
        // Bool Equals Operator Overload
        bool operator==(const Vector3& other) const
        {
            return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
        }
        // Bool Not Equals Operator Overload
        bool operator!=(const Vector3& other) const
        {
            return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
        }
        // Addition Operator Overload
        Vector3 operator+(const Vector3& right) const
        {
            return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
        }
        // Subtraction Operator Overload
        Vector3 operator-(const Vector3& right) const
        {
            return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
        }
        // Float Multiplication Operator Overload
        Vector3 operator*(const float& other) const
        {
            return Vector3(this->X * other, this->Y * other, this->Z * other);
        }
        // Float Division Operator Overload
        Vector3 operator/(const float& other) const
        {
            return Vector3(this->X / other, this->Y / other, this->Z / other);
        }

        // Positional Variables
        float X;
        float Y;
        float Z;
    };

    // Structure: Vertex
    //
    // Description: Model Vertex object that holds
    //	a Position, Normal, and Texture Coordinate
    struct Vertex
    {
        // Position Vector
        Vector3 Position;

        // Normal Vector
        Vector3 Normal;

        // Texture Coordinate Vector
        Vector2 TextureCoordinate;
    };

    struct IndexedVertex
    {
        int PositionIndex;
        int NormalIndex;
        int TexCoordIndex;
    };

    struct Material
    {
        Material()
        {
            Ns = 0.0f;
            Ni = 0.0f;
            d = 0.0f;
            illum = 0;
        }

        // Material Name
        std::string name;
        // Ambient Color
        Vector3 Ka;
        // Diffuse Color
        Vector3 Kd;
        // Specular Color
        Vector3 Ks;
        // Specular Exponent
        float Ns;
        // Optical Density
        float Ni;
        // Dissolve
        float d;
        // Illumination
        int illum;
        // Ambient Texture Map
        std::string map_Ka;
        // Diffuse Texture Map
        std::string map_Kd;
        // Specular Texture Map
        std::string map_Ks;
        // Specular Hightlight Map
        std::string map_Ns;
        // Alpha Texture Map
        std::string map_d;
        // Bump Map
        std::string map_bump;
    };

    // Structure: Mesh
    //
    // Description: A Simple Mesh Object that holds
    //	a name, a vertex list, and an index list
    struct Mesh
    {
        ~Mesh()
        {
            delete[] vertices;
            delete[] indices;
            delete[] realVertices;
        }

        int materialIndex;
        int numVertices;
        IndexedVertex* vertices;
        Vertex* realVertices;

        int numIndices;
        unsigned int* indices;
    };

    class Loader
    {
    public:
        ~Loader()
        {
            Cleanup();
        }

        void Cleanup()
        {
            delete[] mMeshes;
            delete[] mVertPos;
            delete[] mNormals;
            delete[] mTexCoords;
            delete[] mMaterials;

            mNumMeshes = 0;
            mNumVertexPositions = 0;
            mNumNormals = 0;
            mNumTexCoords = 0;
            mNumMaterials = 0;
        }

        Mesh* mMeshes = nullptr;
        int mNumMeshes = 0;
        Vector3* mVertPos = nullptr;
        int mNumVertexPositions = 0;
        Vector3* mNormals = nullptr;
        int mNumNormals = 0;
        Vector2* mTexCoords = nullptr;
        int mNumTexCoords = 0;
        Material* mMaterials = nullptr;
        int mNumMaterials = 0;

        template<class T>
        T Read(char* content, int& currentPos)
        {
            T value;
            memcpy(&value, &content[currentPos], sizeof(T));
            currentPos += sizeof(T);
            return value;
        }

        template<>
        Vector3 Read<Vector3>(char* content, int& currentPos)
        {
            Vector3 value;
            value.X = Read<float>(content, currentPos);
            value.Y = Read<float>(content, currentPos);
            value.Z = Read<float>(content, currentPos);

            return value;
        }

        template<>
        Vector2 Read<Vector2>(char* content, int& currentPos)
        {
            Vector2 value;
            value.X = Read<float>(content, currentPos);
            value.Y = Read<float>(content, currentPos);

            return value;
        }

        template<>
        std::string Read<std::string>(char* content, int& currentPos)
        {
            int length = Read<int>(content, currentPos);
            std::string value(&content[currentPos], length);
            currentPos += length;

            return value;
        }

        template<>
        Material Read<Material>(char* content, int& currentPos)
        {
            Material value;
            value.Ka = Read<Vector3>(content, currentPos);
            value.Kd = Read<Vector3>(content, currentPos);
            value.Ks = Read<Vector3>(content, currentPos);

            value.Ns = Read<float>(content, currentPos);
            value.d = Read<float>(content, currentPos);

            value.illum = Read<int>(content, currentPos);

            value.map_Ka = Read<std::string>(content, currentPos);
            value.map_Kd = Read<std::string>(content, currentPos);
            value.map_Ks = Read<std::string>(content, currentPos);
            value.map_Ns = Read<std::string>(content, currentPos);
            value.map_bump = Read<std::string>(content, currentPos);
            std::string trash = Read<std::string>(content, currentPos);
            trash = Read<std::string>(content, currentPos);
            value.map_d = Read<std::string>(content, currentPos);

            return value;
        }

        bool LoadFile(AAssetManager* assetManager, std::string path) {
            Cleanup();

            AAsset* file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
            // Get the file length
            size_t fileLength = AAsset_getLength(file);

            // Allocate memory to read your file
            char* fileContent = new char[fileLength+1];

            // Read your file
            AAsset_read(file, fileContent, fileLength);
            // For safety you can add a 0 terminating character at the end of your file ...
            fileContent[fileLength] = '\0';

            int currentPos = 0;

            // Data: Num Vertices
//            mNumVertexPositions = Read<int>(fileContent, currentPos);
//            mVertPos = new Vector3[mNumVertexPositions];
//            // Data: Vertices
//            for(int i = 0; i < mNumVertexPositions; i++)
//            {
//                mVertPos[i] = Read<Vector3>(fileContent, currentPos);
//            }
//
//            // Data: Num TexCoords
//            mNumTexCoords = Read<int>(fileContent, currentPos);
//            mTexCoords = new Vector2[mNumTexCoords];
//            // Data: TexCoords
//            for(int i = 0; i < mNumTexCoords; i++)
//            {
//                mTexCoords[i] = Read<Vector2>(fileContent, currentPos);
//            }
//
//            // Data: Num Normals
//            mNumNormals = Read<int>(fileContent, currentPos);
//            mNormals = new Vector3[mNumNormals];
//            // Data: Normals
//            for(int i = 0; i < mNumNormals; i++)
//            {
//                mNormals[i] = Read<Vector3>(fileContent, currentPos);
//            }

            // Data: Num Materials
            mNumMaterials = Read<int>(fileContent, currentPos);
            mMaterials = new Material[mNumMaterials];
            // Data: Materials
            for(int i = 0; i < mNumMaterials; i++)
            {
                mMaterials[i] = Read<Material>(fileContent, currentPos);
            }

            // Data: Num Meshes
            mNumMeshes = Read<int>(fileContent, currentPos);
            mMeshes = new Mesh[mNumMeshes];
            for(int i = 0; i < mNumMeshes; i++)
            {
                // Data: Mesh Material Index
                mMeshes[i].materialIndex = Read<int>(fileContent, currentPos);

                // Data: Num Unique Vertices
                mMeshes[i].numVertices = Read<int>(fileContent, currentPos);
                mMeshes[i].vertices = new IndexedVertex[mMeshes[i].numVertices];
                mMeshes[i].realVertices = new Vertex[mMeshes[i].numVertices];
                // Data: Unique Vertices
                for(int j = 0; j < mMeshes[i].numVertices; j++)
                {
                    mMeshes[i].realVertices[j].Position = Read<Vector3>(fileContent, currentPos);
                    mMeshes[i].realVertices[j].TextureCoordinate = Read<Vector2>(fileContent, currentPos);
                    mMeshes[i].realVertices[j].Normal = Read<Vector3>(fileContent, currentPos);
//                    mMeshes[i].vertices[j].PositionIndex = Read<int>(fileContent, currentPos);
//                    mMeshes[i].vertices[j].TexCoordIndex = Read<int>(fileContent, currentPos);
//                    mMeshes[i].vertices[j].NormalIndex = Read<int>(fileContent, currentPos);
                }

                // Data: Num Vertex Indices
                mMeshes[i].numIndices = Read<int>(fileContent, currentPos);
                mMeshes[i].indices = new unsigned int[mMeshes[i].numIndices];
                // Data: Vertex Indices
                for(int j = 0; j < mMeshes[i].numIndices; j++)
                {
                    mMeshes[i].indices[j] = Read<unsigned int>(fileContent, currentPos);
                }

                // Old parser
                // Data: Num Face Vertices
//                mMeshes[i].numIndices = Read<int>(fileContent, currentPos);
//
//                mMeshes[i].vertexIndex = new int[mMeshes[i].numIndices];
//                mMeshes[i].texIndex = new int[mMeshes[i].numIndices];
//                mMeshes[i].normalIndex = new int[mMeshes[i].numIndices];
//                  // Data: Face Vertex Indices
//                for(int j = 0; j < mMeshes[i].numIndices; j++)
//                {
//                    mMeshes[i].vertexIndex[j] = Read<int>(fileContent, currentPos);
//                    mMeshes[i].texIndex[j] = Read<int>(fileContent, currentPos);
//                    mMeshes[i].normalIndex[j] = Read<int>(fileContent, currentPos);
//                }


            }
            delete[] fileContent;
            return true;
        }
    };
}

#endif //MSP_MOBILE_MODEL_VIEWER_BIN_LOADER_H
