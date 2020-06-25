using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FruitSalad.Model.BMF;
using FruitSalad.Util;

namespace FruitSalad.Model.Collada
{
    class ColladaConverter : IBMFCoverter
    {
        private ColladaModel _model;

        public ColladaConverter(ColladaModel model) 
        {
            _model = model;
        }

        public BMFModel GetModel()
        {
            BMFModel model = new BMFModel();

            Logger.LogMessage("Converting Model...");

            ICollection<string> meshKeys = _model.ColladaMeshDictionary.Keys;
            foreach(string key in meshKeys)
            {
                Logger.LogMessage(string.Format("Processing mesh {0}", key));
                ColladaMesh mesh = _model.ColladaMeshDictionary[key];
                ColladaInput vertexInput = mesh.ColladaInputDictionary["VERTEX"];
                ColladaInput normalInput = mesh.ColladaInputDictionary["NORMAL"];
                ColladaInput texcoordInput = mesh.ColladaInputDictionary["TEXCOORD"];
                int triangleCount = mesh.Triangles.TriangleCount;
                int index = 0;

                IDictionary<int, ColladaVertex> vertexDictionary = new Dictionary<int, ColladaVertex>();
                int lastVertexIndex = 0;
                IList<int> indicesList = new List<int>();
                int duplicateCount = 0;

                for(int i = 0; i < triangleCount; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        int vert = mesh.Triangles.Indices.Indices[index + vertexInput.Offset];
                        int norm = mesh.Triangles.Indices.Indices[index + normalInput.Offset];
                        int texc = mesh.Triangles.Indices.Indices[index + texcoordInput.Offset];

                        IDictionary<string, float> posData = vertexInput.Source.GetAccessor().GetDataFromIndex(vertexInput.Source.GetSource(), vert);
                        IDictionary<string, float> norData = normalInput.Source.GetAccessor().GetDataFromIndex(normalInput.Source.GetSource(), norm);
                        IDictionary<string, float> texData = texcoordInput.Source.GetAccessor().GetDataFromIndex(texcoordInput.Source.GetSource(), texc);

                        float posX = posData["X"];
                        float posY = posData["Y"];
                        float posZ = posData["Z"];
                        float norX = norData["X"];
                        float norY = norData["Y"];
                        float norZ = norData["Z"];
                        float texU = texData["S"];
                        float texV = texData["T"];

                        ColladaVector3 position = new ColladaVector3(posX, posY, posZ);
                        ColladaVector3 normal = new ColladaVector3(norX, norY, norZ);
                        ColladaVector2 uv = new ColladaVector2(texU, texV);

                        ColladaVertex vertex = new ColladaVertex()
                        {
                            Position = position,
                            Normal = normal,
                            UV = uv
                        };

                        //Further Processing can be done here, like tangent calculations and all that stuff
                        //ColladaVector and ColladaVertex can be repurposed to be usuable on other model types aswell, like obj
                        //If this is the case a general export setting like calculate tangents can be calculated after processing the geometry for every type of model

                        bool flag = false;
                        foreach (int dicIndex in vertexDictionary.Keys)
                        {
                            ColladaVertex storedVertex = vertexDictionary[dicIndex];
                            if (vertex == storedVertex)
                            {
                                duplicateCount++;
                                indicesList.Add(dicIndex);
                                flag = true;
                            }

                        }

                        if (!flag)
                        {
                            vertexDictionary.Add(lastVertexIndex, vertex);
                            indicesList.Add(lastVertexIndex);
                            lastVertexIndex++;
                        }

                        index += 3;
                    }
                }

                int[] indices = indicesList.ToArray<int>();
                float[] positions = new float[lastVertexIndex * 3];
                float[] normals = new float[lastVertexIndex * 3];
                float[] texcoords = new float[lastVertexIndex * 2];
                foreach (int vertIndex in vertexDictionary.Keys)
                {
                    ColladaVertex vertex = vertexDictionary[vertIndex];
                    positions[vertIndex * 3 + 0] = vertex.Position.X;
                    positions[vertIndex * 3 + 1] = vertex.Position.Y;
                    positions[vertIndex * 3 + 2] = vertex.Position.Z;

                    normals[vertIndex * 3 + 0] = vertex.Normal.X;
                    normals[vertIndex * 3 + 1] = vertex.Normal.Y;
                    normals[vertIndex * 3 + 2] = vertex.Normal.Z;

                    texcoords[vertIndex * 2 + 0] = vertex.UV.X;
                    texcoords[vertIndex * 2 + 1] = vertex.UV.Y;
                }

                Logger.LogMessage(string.Format("positions Length: {0}", positions.Length));
                Logger.LogMessage(string.Format("normals Length: {0}", normals.Length));
                Logger.LogMessage(string.Format("texcoords Length: {0}", texcoords.Length));
                Logger.LogMessage(string.Format("Indices Length: {0}", indices.Length));
                Logger.LogMessage(string.Format("Number of duplicate vertices found: {0}", duplicateCount));

                model.AddMesh(mesh.ColladaMeshId, indices, positions, normals, texcoords);
                Logger.LogMessage("Successfully converted mesh " + key);
            }

            return model;
        }
    }
}
