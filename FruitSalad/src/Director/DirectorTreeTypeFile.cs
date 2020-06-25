using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Director
{
    public abstract class DirectoryTreeTypeFile : DirectoryTreeType
    {
        private readonly FileType fileType;

        public DirectoryTreeTypeFile(int id, string name, FileType fileType) :
            base(id, name, fileType.IconPath)
        {
            this.fileType = fileType;
        }

        protected override void AddActions()
        {
            AddAction("open", "Open");
        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {
            switch (eventId)
            {
                case "open":
                    FSFile file = new FSFile()
                    {
                        AbosultePath = data.AbsolutePath,
                        Extension = fileType.Extension
                    };
                    FileContainer container = OpenFile(file);
                    FruitSaladApp.Instance.OpenFile(container);
                    break;
            }
        }

        public FileType GetFileType()
        {
            return fileType;
        }

        protected abstract FileContainer OpenFile(FSFile file);
    }
}
