using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Model.Collada
{
    static class ColladaConstants 
    {
		//Basic Nodes
		public const int NODE_TYPE_UNKNOWN = 0;
		public const int NODE_TYPE_ROOT = 1;

		//Asset Nodes
		public const int NODE_TYPE_UNIT = 2;
		public const int NODE_TYPE_UP_AXIS = 3;

		//Geometry Nodes
		public const int NODE_TYPE_LIB_GEOMETRIES = 4;
		public const int NODE_TYPE_GEOMETRY = 5;
		public const int NODE_TYPE_MESH = 6;
		public const int NODE_TYPE_MESH_SOURCE = 7;
		public const int NODE_TYPE_FLOAT_ARRAY = 8;
		public const int NODE_TYPE_ACCESSOR = 9;
		public const int NODE_TYPE_ACCESSOR_PARAM = 10;
		public const int NODE_TYPE_TRIANGLES = 11;
		public const int NODE_TYPE_INPUT = 12;
		public const int NODE_TYPE_TRIANGLE_DATA = 13;

		public const int NODE_TYPE_VERTICES = 14;
	}
    
}
