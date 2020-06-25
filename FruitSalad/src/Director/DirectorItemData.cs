using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Director
{
    public class DirectorItemData
    {
        private string absolutePath;
        private string itemName;
        private DirectoryTreeType itemType;

        public DirectorItemData(string absolutePath, string itemName, DirectoryTreeType itemType)
        {
            this.absolutePath = absolutePath;
            this.itemName = itemName;
            this.itemType = itemType;
        }

        public string AbsolutePath { get { return absolutePath; } }
        public string ItemName { get { return itemName; } }
        public DirectoryTreeType ItemType { get { return itemType; } }
    }
}
