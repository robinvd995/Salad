using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Model.Collada
{
    class ColladaModel
    {
        public ColladaModel()
        {
            ColladaMeshDictionary = new Dictionary<string, ColladaMesh>();
        }

        public IDictionary<string,ColladaMesh> ColladaMeshDictionary { get; private set; }
    }

    class ColladaMesh
    {

        public ColladaMesh()
        {
            ColladaSourceDictionary = new Dictionary<string, IColladaDataSource>();
            ColladaInputDictionary = new Dictionary<string, ColladaInput>();
        }

        public IDictionary<string, IColladaDataSource> ColladaSourceDictionary { get; private set; }
        public IDictionary<string, ColladaInput> ColladaInputDictionary { get; private set; }
        public string ColladaMeshId { get; set; }
        public string ColladaMeshName { get; set; }

        public ColladaTriangles Triangles { get; set; }
    }

    class ColladaSource : IColladaDataSource
    {
        public ColladaSource(int dataLength)
        {
            SourceData = new float[dataLength];
        }

        public string ColladaSourceId { get; set; }
        public ColladaAccessor Accessor { get; set; }
        public float[] SourceData { get; private set; }

        public ColladaAccessor GetAccessor()
        {
            return Accessor;
        }

        public float GetData(int index)
        {
            return SourceData[index];
        }

        public ColladaSource GetSource()
        {
            return this;
        }
    }

    class ColladaAccessor
    {
        public ColladaAccessor()
        {
            ParameterDictionary = new Dictionary<string, int>();
        }

        public int Count { get; set; }
        public int Stride { get; set; }
        public IDictionary<string,int> ParameterDictionary { get; private set; }

        public IDictionary<string,float> GetDataFromIndex(ColladaSource source, int index)
        {
            IDictionary<string,float> data = new Dictionary<string,float>();
            if (index > Count)
            {
                Logger.ErrorMessage("ERROR, Trying to access data from a source outside of its index range!");
                return data;
            }

            foreach(string key in ParameterDictionary.Keys)
            {
                int offset = ParameterDictionary[key];
                int realIndex = index * Stride + offset;
                float value = source.SourceData[realIndex];
                data[key] = value;
            }

            return data;
        }
    }

    struct ColladaVertices : IColladaDataSource
    {
        public string Id { get; set; }
        public ColladaInput Input { get; set; }

        public ColladaAccessor GetAccessor()
        {
            return Input.Source.GetAccessor();
        }

        public float GetData(int index)
        {
            return Input.Source.GetData(index);
        }

        public ColladaSource GetSource()
        {
            return Input.Source.GetSource();
        }
    }

    struct ColladaInput
    {
        public string Semantic { get; set; }
        public string SourceId { get; set; }
        public IColladaDataSource Source { get; set; }
        public int Offset { get; set; }
    }

    class ColladaTriangles
    {
        private ColladaInput[] _inputs;
        private int[] _indices;

        public ColladaTriangles(int count, int inputSize)
        {
            _inputs = new ColladaInput[inputSize];
            _indices = new int[inputSize * 3];
            TriangleCount = count;
        }

        public void AddInput(ColladaInput input)
        {
            _inputs[input.Offset] = input;
        }

        public int GetIndicesCount()
        {
            return _indices.Length;
        }

        public int GetValue(int index)
        {
            return _indices[index];
        }

        public void SetValue(int index, int value)
        {
            _indices[index] = value;
        }
        
        public string Material { get; set; }
        public TriangleData Indices { get; set; }

        public int TriangleCount { get; private set; }
    }

    class TriangleData
    {
        public int[] Indices { get; set; }
    }

    struct ColladaVertex
    {
        public ColladaVector3 Position { get; set; }
        public ColladaVector3 Normal { get; set; }
        public ColladaVector2 UV { get; set; }

        public override bool Equals(object other)
        {
            if(other is ColladaVertex)
            {
                ColladaVertex colladaVertex = (ColladaVertex)other;
                return Compare(this, colladaVertex);
            }
            else
            {
                return false;
            }
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static bool operator ==(ColladaVertex cv0, ColladaVertex cv1)
        {
            return Compare(cv0, cv1);
        }

        public static bool operator !=(ColladaVertex cv0, ColladaVertex cv1)
        {
            return !Compare(cv0, cv1);
        }

        private static bool Compare(ColladaVertex cv0, ColladaVertex cv1)
        {
            return cv0.Position == cv1.Position && cv0.Normal == cv1.Normal && cv0.UV == cv1.UV;
        }
    }

    struct ColladaVector3
    {

        public ColladaVector3(float x, float y, float z) : this()
        {
            X = x;
            Y = y;
            Z = z;
        }

        public float X { get; private set; }
        public float Y { get; private set; }
        public float Z { get; private set; }

        public override bool Equals(object other)
        {
            if (other is ColladaVector3)
            {
                ColladaVector3 vec3 = (ColladaVector3)other;
                return Compare(this, vec3);
            }
            else
            {
                return false;
            }
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static bool operator ==(ColladaVector3 cv0, ColladaVector3 cv1)
        {
            return Compare(cv0, cv1);
        }

        public static bool operator !=(ColladaVector3 cv0, ColladaVector3 cv1)
        {
            return !Compare(cv0, cv1);
        }

        private static bool Compare(ColladaVector3 cv0, ColladaVector3 cv1)
        {
            return cv0.X == cv1.X && cv0.Y == cv1.Y && cv0.Z == cv1.Z;
        }
    }

    struct ColladaVector2
    {
        public ColladaVector2(float x, float y) : this()
        {
            X = x;
            Y = y;
        }

        public float X { get; private set; }
        public float Y { get; private set; }

        public override bool Equals(object other)
        {
            if (other is ColladaVector2)
            {
                ColladaVector2 vec2 = (ColladaVector2)other;
                return Compare(this, vec2);
            }
            else
            {
                return false;
            }
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static bool operator ==(ColladaVector2 cv0, ColladaVector2 cv1)
        {
            return Compare(cv0, cv1);
        }

        public static bool operator !=(ColladaVector2 cv0, ColladaVector2 cv1)
        {
            return !Compare(cv0, cv1);
        }

        private static bool Compare(ColladaVector2 cv0, ColladaVector2 cv1)
        {
            return cv0.X == cv1.X && cv0.Y == cv1.Y;
        }
    }

    interface IColladaDataSource
    {
        float GetData(int index);

        ColladaAccessor GetAccessor();

        ColladaSource GetSource();
    }
}
