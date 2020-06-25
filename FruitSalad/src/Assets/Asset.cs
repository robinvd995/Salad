using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace FruitSalad.Assets
{
    public class Asset
    {
        private string propertiesFilePath;
        private string sourceFilePath;

        public Asset(string filePath, AssetType type)
        {
            Type = type;
            FilePath = filePath;
        }

        public AssetType Type { get; private set; }
        public string FilePath { get; private set; }
        public bool HasProperties { get; private set; }
        public bool HasSource { get; set; }
        public string PropertiesFilePath
        {
            get
            {
                return propertiesFilePath;
            }
            set
            {
                propertiesFilePath = value;
                HasProperties = true;
            }
        }
        public string SourceFilePath
        {
            get
            {
                return sourceFilePath;
            }
            set
            {
                sourceFilePath = value;
                HasSource = true;
            }
        }

        public void ToFile()
        {
            XmlWriterSettings xmlWriterSettings = new XmlWriterSettings()
            {
                Indent = true,
                IndentChars = "    "
            };

            XmlWriter writer = XmlWriter.Create(FilePath, xmlWriterSettings);
            writer.WriteStartDocument();

            writer.WriteStartElement("Asset");

            writer.WriteStartElement("AssetType");
            writer.WriteAttributeString("type", Type.Name);
            writer.WriteEndElement();

            if (HasSource)
            {
                writer.WriteStartElement("SourceFile");
                writer.WriteAttributeString("path", sourceFilePath);
                writer.WriteEndElement();
            }

            if (HasProperties)
            {
                writer.WriteStartElement("PropertiesFile");
                writer.WriteAttributeString("path", propertiesFilePath);
                writer.WriteEndElement();
            }

            writer.WriteEndElement();

            writer.WriteEndDocument();

            writer.Flush();
            writer.Close();
        }

        public static Asset FromFile(string filePath)
        {
            XmlReader reader = XmlReader.Create(filePath);

            AssetType type = null;
            string sourceFile = "";
            string propertiesFile = "";

            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "AssetType"))
                {
                    string typeName = reader.GetAttribute("name");
                    type = Enumeration.GetAll<AssetType>().Where(at => at.Name == typeName).First();
                }

                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "SourceFile"))
                {
                    sourceFile = reader.GetAttribute("path");
                }

                if(reader.NodeType == XmlNodeType.Element && (reader.Name == "PropertiesFile"))
                {
                    propertiesFile = reader.GetAttribute("path");
                }
            }

            Asset asset = new Asset(filePath, type);
            if(sourceFile.Length > 0)
                asset.SourceFilePath = sourceFile;
            if (propertiesFile.Length > 0)
                asset.PropertiesFilePath = propertiesFile;

            return asset;
        }
    }
}
