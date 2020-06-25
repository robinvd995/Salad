using FruitSalad.Director;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeUnknown : DirectoryTreeType
    {
        public DirectoryTreeTypeUnknown(int id, string name, string icon) :
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
