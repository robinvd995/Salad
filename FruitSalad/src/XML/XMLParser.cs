using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace FruitSalad.XML
{
    class XMLParser
    {
        private DataNode _currentNode;

        private void StartNode(XmlReader reader, INodeFactory nodeFactory)
        {
            DataNode node = nodeFactory.CreateDataNode(reader.Name);

            bool b = reader.HasAttributes;
            if (b)
            {

                while (reader.MoveToNextAttribute())
                {
                    string attribName = reader.Name;
                    string attribValue = reader.Value;

                    node.AddAttribute(attribName, attribValue);
                }

                reader.MoveToElement();
            }

            _currentNode.Children.Add(node);
            node.Parent = _currentNode;
            node.OnNodeIntialized();

            _currentNode = node;
        }

        private void LoadData(XmlReader reader)
        {
            _currentNode.LoadData(reader.Value);
        }

        private void FinalizeNode(XmlReader reader)
        {
            _currentNode.OnNodeFinalized();
            _currentNode = _currentNode.Parent;
        }

        public DataNode LoadXMLFile(string filePath, Stream stream, INodeFactory nodeFactory)
        {
            DataNode rootNode = new DataNode(-1);
            _currentNode = rootNode;

            using (XmlReader reader = XmlReader.Create(stream))
            {
                while (reader.Read())
                {
                    bool isStart = reader.IsStartElement();
                    bool isEmpty = reader.IsEmptyElement;

                    bool isSelfClosing = isStart && isEmpty;

                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            StartNode(reader, nodeFactory);

                            if (isSelfClosing)
                            {
                                FinalizeNode(reader);
                            }
                            break;

                        case XmlNodeType.Text:
                            LoadData(reader);
                            break;

                        case XmlNodeType.EndElement:
                            FinalizeNode(reader);
                            break;
                    }
                }

                return rootNode;
            }
        }
    }
}
