using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FruitSalad.Util;
using FruitSalad.XML;

namespace FruitSalad.Model.Collada
{
    class ColladaNodeFloatArray : DataNode
    {
        private string _dataString;
        private float[] _dataArray;

        public ColladaNodeFloatArray(int nodeType) : base(nodeType)
        {
            _dataString = string.Empty;
        }

        public override void LoadData(string data)
        {
            base.LoadData(data);
            if (data.Contains(Environment.NewLine))
            {
                data.Replace(Environment.NewLine, " ");
            }
            _dataString += data;
        }

        public override void OnNodeFinalized()
        {
            if(_dataArray == null)
            {
                Logger.LogMessage("Error while finalizing node Float Array, length of the array was never determined!");
                return;
            }

            string[] splitString = _dataString.Trim().Split(' ');
            int length = splitString.Length;

            if(length != _dataArray.Length)
            {
                Logger.LogMessage("Error while finalizing node Float Array, length of the data does not match the count determined in the attribute!");
                Logger.LogMessage(string.Format("Expected Length {0}, actual Length {1}", _dataArray.Length, length));
                return;
            }

            for(int i = 0; i < length; i++)
            {
                float value = float.Parse(splitString[i]);
                _dataArray[i] = value;
            }

            Logger.LogMessage(string.Format("Successfully loaded {0} data values from float array!", length));
        }

        protected override void OnAttributeAdded(string attribName, string attribValue)
        {
            if (attribName.Equals("count"))
            {
                _dataArray = new float[int.Parse(attribValue)];
            }
        }

        public float[] getArray()
        {
            return _dataArray;
        }
    }

    class ColladaNodeTriangleData : DataNode
    {
        private string _dataString;
        private IList<int> _indices;

        public ColladaNodeTriangleData(int nodeType) : base(nodeType)
        {
            _indices = new List<int>();
        }

        public override void LoadData(string data)
        {
            base.LoadData(data);
            if (data.Contains(Environment.NewLine))
            {
                data.Replace(Environment.NewLine, " ");
            }
            _dataString += data;
        }

        public override void OnNodeFinalized()
        {
            string[] splitString = _dataString.Trim().Split(' ');
            int length = splitString.Length;

            foreach(string s in splitString)
            {
                int value = int.Parse(s);
                _indices.Add(value);
            }
        }

        public IList<int> GetIndices()
        {
            return _indices;
        }
    }
}
