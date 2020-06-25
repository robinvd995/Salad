using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Director
{
    public class DirectorTreeTypeAsset : DirectoryTreeTypeFile
    {
        public DirectorTreeTypeAsset(int id, string name, FileType type) :
            base(id, name, type)
        {

        }

        protected override void AddActions()
        {
            base.AddActions();
            AddAction("build", "Build Asset");
        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {
            base.HandleAction(eventId, data);
            switch (eventId)
            {
                case "build":

                    break;
            }
        }

        protected override FileContainer OpenFile(FSFile file)
        {
            throw new NotImplementedException();
        }
    }
}
