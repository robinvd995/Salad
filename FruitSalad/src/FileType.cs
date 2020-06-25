using FruitSalad.Util;
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
        //public static FileType Workspace = new FileType(1, "workspace", "*", "workspace.png");
        //public static FileType Folder = new FileType(2, "folder", "*", "folder_closed.png");

        public static FileType Asset = new FileType(1, "asset", ".asset", "asset.png");

        public static FileType Model = new FileType(3, "model", ".model", "model.png").SetHasProperties().SetCanEdit().SetIsSourceFile();
        public static FileType Material = new FileType(4, "material", ".material", "material.png").SetHasProperties().SetCanEdit().SetIsSourceFile();
        public static FileType Text = new FileType(5, "text", ".txt", "text.png").SetHasProperties().SetCanEdit().SetIsSourceFile();
        public static FileType Lua = new FileType(6, "lua", ".lua", "script.png").SetHasProperties().SetCanEdit().SetIsSourceFile();
        public static FileType Texture = new FileType(7, "texture", ".png", "image.png").SetHasProperties().SetCanEdit().SetIsSourceFile();
        public static FileType Spline = new FileType(8, "spline", ".spline", "spline.png");

        //public static FileType SourceFolder = new FileType(8, "sourceFolder", "*", "folder_closed.png");
        //public static FileType PropertiesFolder = new FileType(9, "propertiesFolder", "*", "folder_closed.png");
        //public static FileType OuputFolder = new FileType(10, "ouputFolder", "*", "folder_closed.png");

        public static FileType Properties = new FileType(11, "properties", ".properties", "properties.png").SetCanEdit();

        public FileType(int id, string name, string extension, string iconPath)
            : base(id, name)
        {
            Extension = extension;
            IconPath = iconPath;
            HasProperties = false;
        }

        public FileType SetHasProperties()
        {
            HasProperties = true;
            return this;
        }

        public FileType SetCanEdit()
        {
            CanEdit = true;
            return this;
        }

        public FileType SetIsSourceFile()
        {
            IsSourceFile = true;
            return this;
        }

        public FileType SetIsSourceEditable()
        {
            IsSourceEditable = true;
            return this;
        }

        public string Extension { get; private set; }
        public string IconPath { get; private set; }
        public bool HasProperties { get; private set; }
        public PropertyTemplate Template { get; set; }
        public bool CanEdit { get; set; }
        public bool IsSourceFile { get; set; }
        public bool IsSourceEditable { get; set; }
    }
}
