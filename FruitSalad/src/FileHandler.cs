using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{

    public class FileHelper
    {
        public static void LoadProperties(FSFile file, FileType type, FileContainer fileContainer)
        {
            int filePrefixLength = FruitSaladApp.Instance.WorkspaceObj.SourceDirectory.Length;
            string relativeFilePath = file.AbosultePath.Substring(filePrefixLength);
            string propertyPath = FruitSaladApp.Instance.WorkspaceObj.PropertiesDirectory + relativeFilePath + FileType.Properties.Extension;
            Console.WriteLine(propertyPath);
            bool exists = File.Exists(propertyPath);
            if (!exists)
            {
                var dialog = new DialogVMYesNo("Create new properties file", "Could not find an existing properties file.\nDo you want to create a new one?");
                DialogResult result = FruitSaladApp.Instance.GetDialogService().OpenDialog(dialog);
                if (result == DialogResult.Yes)
                {
                    File.Create(propertyPath);
                }
                else
                {
                    return;
                }
            }

            PropertyContainer container = PropertyContainer.FromTemplate(type.Template);
            container.LoadPropertiesFromFile(propertyPath);

            fileContainer.HasPropertyFile = true;
            fileContainer.PropertyFilePath = propertyPath;
            fileContainer.Properties = container;
        }
    }
}
