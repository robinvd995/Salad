using FruitSalad.Assets;
using FruitSalad.Director;
using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeModel : DirectoryTreeTypeFile
    {
        public DirectoryTreeTypeModel(int id, string name, FileType type) :
            base(id, name, type)
        {

        }

        protected override void AddActions()
        {
            base.AddActions();
        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {
            base.HandleAction(eventId, data);
        }

        protected override FileContainer OpenFile(FSFile file)
        {
            throw new NotImplementedException();
        }
    }

    public class AssetTypeModel : AssetType
    {
        public AssetTypeModel(string name, bool hasSource, bool hasProperties) :
            base(name, hasSource, hasProperties)
        {

        }

        public override void BuildAsset(string filePath)
        {
            throw new NotImplementedException();
        }
    }
}
