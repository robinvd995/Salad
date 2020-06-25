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
    public class DirectoryTreeTypeMaterial : DirectoryTreeTypeFile
    {
        public DirectoryTreeTypeMaterial(int id, string name, FileType type) :
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
            FileContainer container = new FileContainer();
            FileHelper.LoadProperties(file, FileType.Material, container);
            container.Source = new SourceContainerMaterial(file.AbosultePath);
            return container;
        }
    }

    public class SourceContainerMaterial : SourceContainer
    {
        private SourceContainerMaterialVM viewModel;

        public SourceContainerMaterial(string filePath) :
            base(filePath)
        {
            viewModel = new SourceContainerMaterialVM();
        }

        public override SourceContainerVM GetViewModel()
        {
            return viewModel;
        }

        public override void SaveSource()
        {
            //throw new NotImplementedException();
        }
    }

    public class SourceContainerMaterialVM : SourceContainerVM
    {
        public override FileType GetFileType()
        {
            return FileType.Material;
        }
    }

    public class AssetTypeMaterial : AssetType
    {
        public AssetTypeMaterial(string name, bool hasSource, bool hasProperties) :
            base(name, hasSource, hasProperties)
        {

        }

        public override void BuildAsset(string filePath)
        {
            throw new NotImplementedException();
        }
    }
}
