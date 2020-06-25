using FruitSalad.Director;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeAssetsFolder : DirectoryTreeType
    {
        public DirectoryTreeTypeAssetsFolder(int id, string name, string icon) :
            base(id, name, icon)
        {

        }

        protected override void AddActions()
        {
            AddAction("add_asset", "Add Asset");
        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {
            switch (eventId)
            {
                case "add_asset":
                    NewAssetWindow newAssetWindow = new NewAssetWindow();
                    newAssetWindow.ShowDialog();
                    break;
            }
        }
    }
}
