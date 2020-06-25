using FruitSalad.Assets;
using FruitSalad.Director;
using FruitSalad.Util;
using ICSharpCode.AvalonEdit.Document;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeLua : DirectoryTreeTypeFile
    {
        public DirectoryTreeTypeLua(int id, string name, FileType type) :
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
            container.Source = new SourceContainerLua(file.AbosultePath);
            return container;
        }
    }

    public class SourceContainerLua : SourceContainer
    {
        private SourceContainerLuaVM viewModel;
        public string Content { get; set; }

        public SourceContainerLua(string filePath) :
            base(filePath)
        {
            Content = File.ReadAllText(filePath);
            viewModel = new SourceContainerLuaVM(this);
        }

        public override SourceContainerVM GetViewModel()
        {
            return viewModel;
        }

        public override void SaveSource()
        {
            File.WriteAllText(FilePath, viewModel.Document.Text);
        }
    }

    public class SourceContainerLuaVM : SourceContainerVM
    {
        private SourceContainerLua container;

        public SourceContainerLuaVM(SourceContainerLua containerLua)
        {
            EditorTitle = "Lua Editor";
            container = containerLua;
            Document = new TextDocument
            {
                Text = containerLua.Content
            };
        }

        public override FileType GetFileType()
        {
            return FileType.Lua;
        }

        public TextDocument Document { get; set; }
    }

    public class AssetTypeLua : AssetType
    {
        public AssetTypeLua(string name, bool hasSource, bool hasProperties) :
            base(name, hasSource, hasProperties)
        {

        }

        public override void BuildAsset(string filePath)
        {
            throw new NotImplementedException();
        }
    }
}
