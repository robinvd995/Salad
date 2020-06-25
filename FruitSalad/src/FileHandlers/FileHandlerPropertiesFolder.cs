using FruitSalad.Director;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypePropertiesFolder : DirectoryTreeType
    {
        public DirectoryTreeTypePropertiesFolder(int id, string name, string icon) :
            base(id, name, icon)
        {

        }

        protected override void AddActions()
        {

        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {

        }
    }
}
