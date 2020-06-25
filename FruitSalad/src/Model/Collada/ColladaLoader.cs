using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;
using FruitSalad.Model.BMF;
using FruitSalad.Util;
using FruitSalad.XML;

namespace FruitSalad.Model.Collada
{
    class ColladaLoader
    {

        public static BMFModel LoadColladaFile(string filePath, Stream stream)
        {

            ColladaModel model = new ColladaModel();

            Logger.LogMessage(string.Format("Loading file {0} ...", filePath));
            DataNode rootNode = new XMLParser().LoadXMLFile(filePath, stream, new NodeFactory());
            Logger.LogMessage("Loading file into data structure...");

            IEnumerator<DataNode> geometryEnum = rootNode.GetFilteredEnumerator(GeometryFilter);
            while (geometryEnum.MoveNext())
            {
                DataNode node = geometryEnum.Current;
                string geometryId = node.GetAttribute("id");
                string geometryName = node.GetAttribute("name");
                Logger.LogMessage(string.Format("Found geometry with id {0} and name {1}", geometryId, geometryName));

                ColladaMesh colladaMesh = new ColladaMesh()
                {
                    ColladaMeshId = geometryId,
                    ColladaMeshName = geometryName
                };

                LoadColladaMesh(node, ref colladaMesh);
                model.ColladaMeshDictionary.Add(colladaMesh.ColladaMeshId, colladaMesh);
            }

            ColladaConverter converter = new ColladaConverter(model);
            return converter.GetModel();
        }

        private static void LoadColladaMesh(DataNode meshRootNode, ref ColladaMesh mesh)
        {
            //Loading sources
            Logger.LogMessage("Loading mesh source data...");
            IEnumerator<DataNode> sourceNodes = meshRootNode.GetFilteredEnumerator(SourceFilter);
            while (sourceNodes.MoveNext())
            {
                DataNode sourceNode = sourceNodes.Current;
                string sourceId = sourceNode.GetAttribute("id");
                Logger.LogMessage(string.Format("Found source with id {0}!", sourceId));
                DataNode fan = sourceNode.GetSingle(ColladaConstants.NODE_TYPE_FLOAT_ARRAY);
                if (fan == null)
                {
                    Logger.LogMessage(string.Format("Error loading float array from source with id {0}!", sourceId));
                    continue;
                }

                ColladaNodeFloatArray floatArrayNode = (ColladaNodeFloatArray) fan;
                float[] data = floatArrayNode.getArray();
                int length = data.Length;
                ColladaSource source = new ColladaSource(length);
                for (int i = 0; i < length; i++)
                {
                    source.SourceData[i] = data[i];
                }

                DataNode accessorNode = sourceNode.GetSingle(ColladaConstants.NODE_TYPE_ACCESSOR);
                int count = int.Parse(accessorNode.GetAttribute("count"));
                int stride = int.Parse(accessorNode.GetAttribute("stride"));
                ColladaAccessor accesssor = new ColladaAccessor()
                {
                    Count = count,
                    Stride = stride
                };
                Logger.LogMessage(string.Format("Loading accessor for source {0}...", sourceId));

                IEnumerator<DataNode> enumeratorNodes = accessorNode.GetFilteredEnumerator(ParamFilter);
                int offset = 0;
                while (enumeratorNodes.MoveNext())
                {
                    DataNode param = enumeratorNodes.Current;
                    string paramName = param.GetAttribute("name");
                    Logger.LogMessage(string.Format("Loaded parameter {0} with offset {1} for accessor {2}!", paramName, offset, sourceId));
                    accesssor.ParameterDictionary.Add(paramName, offset);
                    offset++;
                }

                source.Accessor = accesssor;
                mesh.ColladaSourceDictionary.Add(sourceId, source);

            }

            //Loading the vertices
            DataNode verticesNode = meshRootNode.GetSingle(ColladaConstants.NODE_TYPE_VERTICES);
            DataNode verticesInputNode = verticesNode.GetSingle(ColladaConstants.NODE_TYPE_INPUT);
            string vertInputSource = verticesInputNode.GetAttribute("source").Remove(0, 1);
            string vertInputSemantic = verticesInputNode.GetAttribute("semantic");
            ColladaInput vertexInput = new ColladaInput()
            {
                Semantic = vertInputSemantic,
                Offset = 0,
                SourceId = vertInputSource,
                Source = mesh.ColladaSourceDictionary[vertInputSource]
            };

            ColladaVertices vertices = new ColladaVertices()
            {
                Id = verticesNode.GetAttribute("id"),
                Input = vertexInput
            };

            Logger.LogMessage(string.Format("Loaded vertices {0} with source {1}!", vertInputSemantic, vertInputSource));

            mesh.ColladaSourceDictionary.Add(vertices.Id, vertices);
            mesh.ColladaInputDictionary.Add(vertInputSemantic, vertexInput);

            //Loading triangles
            DataNode triangleNode = meshRootNode.GetSingle(ColladaConstants.NODE_TYPE_TRIANGLES);
            string triangleMaterial = triangleNode.GetAttribute("material");
            int triangleCount = int.Parse(triangleNode.GetAttribute("count"));

            Logger.LogMessage(string.Format("Loading triangles with material {0}, triangle count = {1}", triangleMaterial, triangleCount));

            IEnumerator<DataNode> triangleInputNodes = triangleNode.GetFilteredEnumerator(InputFilter);
            int tic = 0;
            IList<ColladaInput> triangleInputList = new List<ColladaInput>();
            while (triangleInputNodes.MoveNext())
            {
                DataNode inputNode = triangleInputNodes.Current;
                string inputSouceId = inputNode.GetAttribute("source").Remove(0, 1);
                string inputSemantic = inputNode.GetAttribute("semantic");
                int inputOffset = int.Parse(inputNode.GetAttribute("offset"));
                IColladaDataSource inputSource = mesh.ColladaSourceDictionary[inputSouceId];
                ColladaInput input = new ColladaInput()
                {
                    Semantic = inputSemantic,
                    SourceId = inputSouceId,
                    Source = inputSource,
                    Offset = inputOffset

                };

                mesh.ColladaInputDictionary.Add(inputSemantic, input);
                Logger.LogMessage(string.Format("Loaded triangle input {0} with source {1} and offset {2}", inputSemantic, inputSouceId, inputOffset));

                tic++;
            }

            ColladaTriangles triangles = new ColladaTriangles(triangleCount, tic)
            {
                Material = triangleMaterial
            };

            foreach(ColladaInput cin in triangleInputList)
            {
                triangles.AddInput(cin);
            }

            TriangleData tData = new TriangleData()
            {
                Indices = new int[triangleCount * tic * 3]
            };

            Logger.LogMessage(string.Format("Loading triangle data, values count = {0}", triangleCount * tic * 3));
            ColladaNodeTriangleData nodeTriangleData = (ColladaNodeTriangleData) triangleNode.GetSingle(ColladaConstants.NODE_TYPE_TRIANGLE_DATA);
            int index = 0;
            foreach (int i in nodeTriangleData.GetIndices())
            {
                tData.Indices[index] = i;
                index++;
            }

            triangles.Indices = tData;
            mesh.Triangles = triangles;

            Logger.LogMessage(string.Format("Successfully load mesh with id {0}!", mesh.ColladaMeshId));
        }

        private static bool TriangleFilter(DataNode obj)
        {
            return obj.NodeType == ColladaConstants.NODE_TYPE_TRIANGLES;
        }

        private static bool GeometryFilter(DataNode obj)
        {
            return obj.NodeType == ColladaConstants.NODE_TYPE_GEOMETRY;
        }

        private static bool SourceFilter(DataNode obj)
        {
            return obj.NodeType == ColladaConstants.NODE_TYPE_MESH_SOURCE;
        }

        private static bool ParamFilter(DataNode obj)
        {
            return obj.NodeType == ColladaConstants.NODE_TYPE_ACCESSOR_PARAM;
        }

        private static bool InputFilter(DataNode obj)
        {
            return obj.NodeType == ColladaConstants.NODE_TYPE_INPUT;
        }

        class NodeFactory : INodeFactory
        {
            public DataNode CreateDataNode(string nodeName)
            {
                switch (nodeName)
                {
                    default:                    return new DataNode(ColladaConstants.NODE_TYPE_UNKNOWN);
                    case "COLLADA":             return new DataNode(ColladaConstants.NODE_TYPE_ROOT);
                    case "library_geometries":  return new DataNode(ColladaConstants.NODE_TYPE_LIB_GEOMETRIES);
                    case "geometry":            return new DataNode(ColladaConstants.NODE_TYPE_GEOMETRY);
                    case "mesh":                return new DataNode(ColladaConstants.NODE_TYPE_MESH);
                    case "source":              return new DataNode(ColladaConstants.NODE_TYPE_MESH_SOURCE);
                    case "float_array":         return new ColladaNodeFloatArray(ColladaConstants.NODE_TYPE_FLOAT_ARRAY);
                    case "asset":               return new DataNode(ColladaConstants.NODE_TYPE_UNKNOWN);
                    case "triangles":           return new DataNode(ColladaConstants.NODE_TYPE_TRIANGLES);
                    case "input":               return new DataNode(ColladaConstants.NODE_TYPE_INPUT);
                    case "p":                   return new ColladaNodeTriangleData(ColladaConstants.NODE_TYPE_TRIANGLE_DATA);
                    case "accessor":            return new DataNode(ColladaConstants.NODE_TYPE_ACCESSOR);
                    case "param":               return new DataNode(ColladaConstants.NODE_TYPE_ACCESSOR_PARAM);
                    case "vertices":            return new DataNode(ColladaConstants.NODE_TYPE_VERTICES);
                }
            }
        }
    }
}
