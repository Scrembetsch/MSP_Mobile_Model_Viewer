using ObjLoader.Loader;
using ObjLoader.Loader.Data;
using ObjLoader.Loader.Data.Elements;
using ObjLoader.Loader.Data.VertexData;
using ObjLoader.Loader.Loaders;
using System.Linq;

namespace App
{
    public static class Program
    {
        public static void Main(string[] _)
        {
            string currentDir = Directory.GetCurrentDirectory() + "\\..\\..\\..\\..\\";
            string targetDir = currentDir + "app\\src\\main\\assets\\models\\";

            // Convert obj & mtl files
            RecursiveObjConversion(targetDir);
        }

        private static void RecursiveObjConversion(string targetDir)
        {
            foreach (string dir in Directory.GetDirectories(targetDir))
            {
                string dirName = GetDirName(dir);
                RecursiveObjConversion(targetDir + dirName);
            }

            foreach (string file in Directory.GetFiles(targetDir))
            {
                if(file.EndsWith(".obj"))
                {
                    string fileName = GetFileName(file);
                    ConvertObjAndMtl(file[..^fileName.Length], fileName);
                }
            }
        }

        #region File & Dir Utils
        public static string GetDirName(string dirPath)
        {
            string[] splits = dirPath.Split('\\');
            return splits[^1] + "\\";
        }

        public static string GetFileName(string filePath)
        {
            string[] splits = filePath.Split('\\');
            return splits[^1];
        }

        public static void RecursiveCopy(string sourceDir, string targetDir, bool overrideFile)
        {
            foreach(string dir in Directory.GetDirectories(sourceDir))
            {
                string dirName = GetDirName(dir);
                RecursiveCopy(sourceDir + dirName, targetDir + dirName, overrideFile);
            }

            Directory.CreateDirectory(targetDir);
            foreach(string file in Directory.GetFiles(sourceDir))
            {
                string fileName = GetFileName(file);
                File.Copy(sourceDir + fileName, targetDir + fileName, overrideFile);
            }
        }
        #endregion

        public static void ConvertObjAndMtl(string path, string objName)
        {
            string currentDir = Directory.GetCurrentDirectory();
            try
            {
                Directory.SetCurrentDirectory(path);
                Console.WriteLine($"Parsing: " + objName);
                DateTime start = DateTime.Now;
                FileStream readStream = new FileStream(objName, FileMode.Open, FileAccess.Read);
                LoadResult objResult = new ObjLoaderFactory().Create().Load(readStream);
                string newFile = path + objName[0..^3] + "leb";
                File.Delete(newFile);
                FileStream fileStream = new FileStream(newFile, FileMode.OpenOrCreate, FileAccess.Write);
                BinaryWriter bw = new BinaryWriter(fileStream);
                bw.Write(objResult.Materials);

                // Data: Num Meshes
                bw.Write(objResult.Groups.Count);
                foreach(var group in objResult.Groups)
                {
                    int index = objResult.Materials.IndexOf(group.Material);
                    // Data: Mesh Material Index
                    bw.Write(objResult.Materials.IndexOf(group.Material));

                    IList<Face> faces = GroupAndTriangulateFaces(group.Faces, objResult.Vertices);
                    RemoveDuplicateVerticesAndIndexList(faces, out IList<FaceVertex> uniqueVertices, out IList<uint> indices);

                    // Data: Num Unique Vertices
                    // Data: Unique Vertices
                    bw.Write(uniqueVertices, objResult.Vertices, objResult.Textures, objResult.Normals);
                    // Data: Num Vertex Indices
                    // Data: Vertex Indices
                    bw.Write(indices);
                }
                bw.Flush();
                fileStream.Close();

                Console.WriteLine($"Finished after: {(DateTime.Now - start).Milliseconds} ms");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine(ex.StackTrace);
            }
            Directory.SetCurrentDirectory(currentDir);
        }

        public static IList<Face> GroupAndTriangulateFaces(IList<Face> faces, IList<Vertex> vertices)
        {
            List<Face> ret = new List<Face>();

            foreach (var face in faces)
            {
                if(face.Count == 3)
                {
                    ret.Add(face);
                }
                else if(face.Count == 4)
                {
                    ret.AddRange(TriangulateQuad(face, vertices));
                }
                else
                {
                    Console.WriteLine("Invalid Face!");
                }
            }
            return ret;
        }

        public static void RemoveDuplicateVerticesAndIndexList(IList<Face> faces, out IList<FaceVertex> uniqueVertices, out IList<uint> indices)
        {
            Dictionary<FaceVertex, uint> vertexSet = new();
            uniqueVertices = new List<FaceVertex>();
            indices = new List<uint>();

            foreach (var face in faces)
            {
                for (int i = 0; i < face.Count; i++)
                {
                    if(vertexSet.ContainsKey(face[i]))
                    {
                        indices.Add(vertexSet[face[i]]);
                    }
                    else
                    {
                        vertexSet.Add(face[i], (uint)uniqueVertices.Count);
                        indices.Add((uint)uniqueVertices.Count);

                        uniqueVertices.Add(face[i]);
                    }
                }
            }
        }

        public static Face[] TriangulateQuad(Face face, IList<Vertex> vertices)
        {
            Face[] ret = new Face[2];
            ret[0] = new Face();
            ret[1] = new Face();
            float dist1 = vertices[face[0].VertexIndex - 1].DistanceTo(vertices[face[2].VertexIndex - 1]);
            float dist2 = vertices[face[1].VertexIndex - 1].DistanceTo(vertices[face[3].VertexIndex - 1]);
            if (dist1 > dist2)
            {
                ret[0].AddVertex(face[0]);
                ret[0].AddVertex(face[1]);
                ret[0].AddVertex(face[3]);
                ret[1].AddVertex(face[1]);
                ret[1].AddVertex(face[2]);
                ret[1].AddVertex(face[3]);
            }
            else
            {
                ret[0].AddVertex(face[0]);
                ret[0].AddVertex(face[1]);
                ret[0].AddVertex(face[2]);
                ret[1].AddVertex(face[0]);
                ret[1].AddVertex(face[2]);
                ret[1].AddVertex(face[3]);
            }
            return ret;
        }

        public static float DistanceTo(this Vertex vert, Vertex other)
        {
            return Math.Abs(vert.X - other.X) +
                Math.Abs(vert.Y - other.Y) +
                Math.Abs(vert.Z - other.Z);
        }

        public static void Write(this BinaryWriter bw, IList<Vertex> value)
        {
            bw.Write(value.Count);
            foreach(var val in value)
            {
                bw.Write(val);
            }
        }

        public static void Write(this BinaryWriter bw, Vertex value)
        {
            bw.Write(value.X);
            bw.Write(value.Y);
            bw.Write(value.Z);
        }

        public static void Write(this BinaryWriter bw, IList<Texture> value)
        {
            bw.Write(value.Count);
            foreach (var val in value)
            {
                bw.Write(val);
            }
        }

        public static void Write(this BinaryWriter bw, Texture value)
        {
            bw.Write(value.X);
            bw.Write(value.Y);
        }

        public static void Write(this BinaryWriter bw, IList<Normal> value)
        {
            bw.Write(value.Count);
            foreach (var val in value)
            {
                bw.Write(val);
            }
        }

        public static void Write(this BinaryWriter bw, Normal value)
        {
            bw.Write(value.X);
            bw.Write(value.Y);
            bw.Write(value.Z);
        }

        public static void Write(this BinaryWriter bw, IList<Material> value)
        {
            bw.Write(value.Count);
            foreach (var val in value)
            {
                bw.Write(val);
            }
        }

        public static void Write(this BinaryWriter bw, Material value)
        {
            bw.Write(value.AmbientColor);
            bw.Write(value.DiffuseColor);
            bw.Write(value.SpecularColor);

            bw.Write(value.SpecularCoefficient);
            bw.Write(value.Transparency);

            bw.Write(value.IlluminationModel);

            bw.WriteString(value.AmbientTextureMap);
            bw.WriteString(value.DiffuseTextureMap);
            bw.WriteString(value.SpecularTextureMap);
            bw.WriteString(value.SpecularHighlightTextureMap);
            bw.WriteString(value.BumpMap);
            bw.WriteString(value.DisplacementMap);
            bw.WriteString(value.StencilDecalMap);
            bw.WriteString(value.AlphaTextureMap);
        }

        public static void Write(this BinaryWriter bw, IList<FaceVertex> value, IList<Vertex> positions, IList<Texture> texCoords, IList<Normal> normals)
        {
            bw.Write(value.Count);
            foreach (var val in value)
            {
                bw.Write(val, positions, texCoords, normals);
            }
        }

        public static void Write(this BinaryWriter bw, FaceVertex value, IList<Vertex> positions, IList<Texture> texCoords, IList<Normal> normals)
        {
            bw.Write(positions[value.VertexIndex - 1]);
            bw.Write(texCoords[value.TextureIndex - 1]);
            bw.Write(normals[value.NormalIndex - 1]);
        }

        public static void Write(this BinaryWriter bw, IList<uint> value)
        {
            bw.Write(value.Count);
            foreach (var val in value)
            {
                bw.Write(val);
            }
        }

        public static void WriteString(this BinaryWriter bw, string value)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                bw.Write(0);
            }
            else
            {
                bw.Write(value.Length);
                bw.Write(value.ToCharArray());
            }
        }

        public static void Write(this BinaryWriter bw, Vec3 value)
        {
            bw.Write(value.X);
            bw.Write(value.Y);
            bw.Write(value.Z);
        }
    }
}

