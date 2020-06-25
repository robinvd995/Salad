using FruitSalad.FileHandlers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Director
{
    public abstract class DirectoryTreeType : Enumeration
    {
        public static DirectoryTreeType Unknown = new DirectoryTreeTypeUnknown(0, "unknown", "unknown.png");
        public static DirectoryTreeType Workspace = new DirectoryTreeTypeWorkspace(1, "workspace", "workspace.png");
        public static DirectoryTreeType Folder = new DirectoryTreeTypeFolder(2, "folder", "folder_closed.png");

        public static DirectoryTreeType AssetsFolder = new DirectoryTreeTypeAssetsFolder(12, "Assets", "folder_closed.png");
        public static DirectoryTreeType SourceFolder = new DirectoryTreeTypeSourceFolder(3, "Source", "folder_closed.png");
        public static DirectoryTreeType PropertiesFolder = new DirectoryTreeTypePropertiesFolder(4, "Properties", "folder_closed.png");
        public static DirectoryTreeType OutputFolder = new DirectoryTreeTypeOutputFolder(5, "Output", "folder_closed.png");

        public static DirectoryTreeType Model = new DirectoryTreeTypeModel(6, "model", FileType.Model);
        public static DirectoryTreeType Material = new DirectoryTreeTypeMaterial(7, "material", FileType.Material);
        public static DirectoryTreeType Text = new DirectoryTreeTypeText(8, "text", FileType.Text);
        public static DirectoryTreeType Script = new DirectoryTreeTypeLua(9, "lua", FileType.Lua);
        public static DirectoryTreeType Texture = new DirectoryTreeTypeTexture(10, "texture", FileType.Texture);

        public static DirectoryTreeType Spline = new DirectoryTreeTypeSpline(13, "spline", FileType.Spline);

        public static DirectoryTreeType Properties = new DirectoryTreeTypeProperties(11, "properties", FileType.Properties);
        public static DirectoryTreeType Asset = new DirectorTreeTypeAsset(12, "asset", FileType.Asset);

        private readonly string iconPath;

        public DirectoryTreeType(int id, string name, string icon)
            : base(id, name)
        {
            iconPath = "res/16_16/" + icon;
            ActionList = new List<Tuple<string, string>>();
            AddActions();
        }

        public virtual string GetIcon()
        {
            return iconPath;
        }

        protected void AddAction(string id, string name)
        {
            ActionList.Add(new Tuple<string, string>(id, name));
        }

        protected abstract void AddActions();

        public abstract void HandleAction(string eventId, DirectorItemData data);

        public List<Tuple<string, string>> ActionList { get; set; }
    }
}
