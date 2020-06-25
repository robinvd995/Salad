using FruitSalad.FileHandlers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Assets
{
    public abstract class AssetType : Enumeration
    {
        private static int enumerator = 0;

        public static readonly AssetType Material = new AssetTypeMaterial("Material", false, true);
        public static readonly AssetType Model = new AssetTypeModel("Model", true, true);
        public static readonly AssetType Lua = new AssetTypeLua("Lua", true, false);

        public AssetType(string name, bool hasSource, bool hasProperties) :
            base(enumerator++, name)
        {
            HasSource = hasSource;
            HasProperties = hasProperties;
        }

        public bool HasSource { get; private set; }
        public bool HasProperties { get; private set; }
        public abstract void BuildAsset(string filePath);
    }
}
