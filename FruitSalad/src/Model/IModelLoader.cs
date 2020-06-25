using System.IO;
using FruitSalad.Model.BMF;

namespace FruitSalad.Model
{
    interface IModelLoader
    {
        BMFModel LoadFile(string filePath, Stream stream);
    }
}
