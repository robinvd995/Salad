using System;
using System.Collections.Generic;
using System.IO;

namespace FruitSalad.Model.BMF
{
    static class BMFExporter
    {
        private static byte[] bytebuf = new byte[4];

        public static void WriteFile(BMFModel model, string filePath)
        {
            using (var fs = new FileStream(filePath, FileMode.Create, FileAccess.Write))
            {
                IReadOnlyList<BMFMesh> meshes = model.GetBMFMeshes();
                fs.WriteByte((byte)meshes.Count);
                foreach (BMFMesh mesh in meshes)
                {
                    fs.WriteByte((byte)mesh.MeshID.Length);
                    foreach (char c in mesh.MeshID)
                    {
                        fs.WriteByte((byte)c);
                    }

                    WriteIntArray(fs, mesh.Indices);
                    WriteFloatArray(fs, mesh.Positions);
                    WriteFloatArray(fs, mesh.Normals);
                    WriteFloatArray(fs, mesh.Uvs);
                }

                fs.Flush();
            }
        }

        private static void WriteInt(FileStream fs, int value)
        {
            bytebuf = BitConverter.GetBytes(value);
            //Array.Reverse(bytebuf);
            fs.Write(bytebuf, 0, 4);
        }

        private static void WriteFloat(FileStream fs, float value)
        {
            bytebuf = BitConverter.GetBytes(value);
            fs.Write(bytebuf, 0, 4);
        }

        private static void WriteIntArray(FileStream fs, int[] array)
        {
            WriteInt(fs, array.Length);
            foreach(int value in array)
            {
                WriteInt(fs, value);
            }
        }

        private static void WriteFloatArray(FileStream fs, float[] array)
        {
            WriteInt(fs, array.Length);
            foreach (float value in array)
            {
                WriteFloat(fs, value);
            }
        }
    }
}
