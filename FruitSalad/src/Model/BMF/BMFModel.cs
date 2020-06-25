using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Model.BMF
{
    class BMFModel
    {
        private List<BMFMesh> _meshList;

        public BMFModel()
        {
            _meshList = new List<BMFMesh>();
        }

        public void AddMesh(string id, int[] indices, float[] positions, float[] normals, float[] uvs)
        {
            int posVertCount = positions.Length / 3;
            int norVertCount = normals.Length / 3;
            int uvsVertCount = uvs.Length / 2;

            if(posVertCount != norVertCount || norVertCount != uvsVertCount)
            {
                Logger.ErrorMessage(string.Format("ERROR positions, normals and uvs for mesh with id {0} do not have the same length!", id));
                Logger.ErrorMessage(string.Format("positions length = {0}, normals length = {1}, uvs length = {2}", posVertCount, norVertCount, uvsVertCount));
                Logger.ErrorMessage(string.Format("Skipping mesh with id {0}", id));
                return;
            }

            BMFMesh mesh = new BMFMesh()
            {
                MeshID = id,
                Positions = positions,
                Normals = normals,
                Uvs = uvs,
                Indices = indices
            };

            _meshList.Add(mesh);
        }

        public IReadOnlyList<BMFMesh> GetBMFMeshes() { return _meshList.AsReadOnly(); }
    }

    class BMFMesh
    {
        public string MeshID { get; set; }
        public float[] Positions { get; set;  }
        public float[] Normals { get; set; }
        public float[] Uvs { get; set; }
        public int[] Indices { get; set; }
    }
}
