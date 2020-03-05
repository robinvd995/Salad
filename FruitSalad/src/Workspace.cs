using System;
using System.IO;
using System.Xml;

namespace FruitSalad
{
    class Workspace
    {
        public string Name { get; private set; }
        public string WorkingDirectory { get; private set; }
        public string SourceDirectory { get; private set; }
        public string PropertiesDirectory { get; private set; }
        public string OutputDirectory { get; private set; }

        public static Workspace GetWorkspace(string filepath)
        {
            Workspace workspace = new Workspace();

            XmlReader reader = XmlReader.Create(filepath);
            while (reader.Read())
            {
                if(reader.NodeType == XmlNodeType.Element && (reader.Name == "Name"))
                {
                    workspace.Name = reader.GetAttribute("value");
                }

                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "Directory"))
                {
                    workspace.WorkingDirectory = reader.GetAttribute("path");
                }

                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "SourceDirectory"))
                {
                    workspace.SourceDirectory = reader.GetAttribute("path");
                }

                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "PropertiesDirectory"))
                {
                    workspace.PropertiesDirectory = reader.GetAttribute("path");
                }

                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "OutputDirectory"))
                {
                    workspace.OutputDirectory = reader.GetAttribute("path");
                }
            }

            return workspace;
        }

        public static string CreateNewWorkspace(string workspaceName, string directory, string sourceDir, string propertyDir, string outputDir)
        {
            //Making directories
            Directory.CreateDirectory(directory);
            Directory.CreateDirectory(sourceDir);
            Directory.CreateDirectory(propertyDir);
            Directory.CreateDirectory(outputDir);

            string workspaceFile = directory + workspaceName + ".fsws";
            XmlWriter writer = XmlWriter.Create(workspaceFile);
            writer.WriteStartDocument();

            writer.WriteStartElement("Root");

            writer.WriteStartElement("Name");
            writer.WriteAttributeString("value", workspaceName);
            writer.WriteEndElement();

            writer.WriteStartElement("Directory");
            writer.WriteAttributeString("path", directory);
            writer.WriteEndElement();

            writer.WriteStartElement("SourceDirectory");
            writer.WriteAttributeString("path", sourceDir);
            writer.WriteEndElement();

            writer.WriteStartElement("PropertiesDirectory");
            writer.WriteAttributeString("path", propertyDir);
            writer.WriteEndElement();

            writer.WriteStartElement("OutputDirectory");
            writer.WriteAttributeString("path", outputDir);
            writer.WriteEndElement();

            writer.WriteEndElement();

            writer.WriteEndDocument();
            writer.Close();

            return workspaceFile;
        }
    }
}
