using FruitSalad.Util;
using FruitSalad.Director;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using ICSharpCode.AvalonEdit.Document;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeProperties : DirectoryTreeTypeFile
    {
        public DirectoryTreeTypeProperties(int id, string name, FileType type) :
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
            container.Source = new SourceContainerProperties(file.AbosultePath);
            return container;
        }
    }

    public class SourceContainerProperties : SourceContainer
    {
        private SourceContainerPropertiesVM viewModel;
        public string Content { get; set; }

        public SourceContainerProperties(string filePath) :
            base(filePath)
        {
            Content = File.ReadAllText(filePath);
            viewModel = new SourceContainerPropertiesVM(this);
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

    public class SourceContainerPropertiesVM : SourceContainerVM
    {
        private SourceContainerProperties container;

        public SourceContainerPropertiesVM(SourceContainerProperties containerProperties)
        {
            EditorTitle = "Properties Editor";
            container = containerProperties;
            Document = new TextDocument
            {
                Text = containerProperties.Content
            };
        }

        public override FileType GetFileType()
        {
            return FileType.Properties;
        }

        public TextDocument Document { get; set; }
    }
}
