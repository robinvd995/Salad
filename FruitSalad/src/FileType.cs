using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class FileType : Enumeration
    {
        public static FileType Unknown = new FileType(0, "unknown", "*", "unknown.png");
        public static FileType Workspace = new FileType(1, "workspace", "*", "workspace.png");
        public static FileType Folder = new FileType(2, "folder", "*", "folder_closed.png");
        public static FileType Model = new FileType(3, "model", ".model", "model.png").SetIsProperty();
        public static FileType Material = new FileType(4, "material", ".material", "material.png").SetIsProperty();
        public static FileType Text = new FileType(5, "text", ".txt", "text.png").SetIsProperty();
        public static FileType Script = new FileType(6, "script", ".lua", "script.png").SetIsProperty();
        public static FileType Texture = new FileType(7, "texture", ".png", "image.png").SetIsProperty();

        public static FileType SourceFolder = new FileType(8, "SourceFolder", "*", "folder_closed.png");
        public static FileType PropertiesFolder = new FileType(9, "PropertiesFolder", "*", "folder_closed.png");
        public static FileType OuputFolder = new FileType(10, "OuputFolder", "*", "folder_closed.png");

        public static FileType Properties = new FileType(11, "Properties", ".properties", "properties.png");

        public FileType(int id, string name, string extension, string iconPath)
            : base(id, name)
        {
            Extension = extension;
            IconPath = iconPath;
            IsProperty = false;
        }

        public FileType SetIsProperty()
        {
            IsProperty = true;
            return this;
        }

        public string Extension { get; private set; }
        public string IconPath { get; private set; }
        public bool IsProperty { get; private set; }
        public PropertyTemplate Template { get; set; }
    }
}
