using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;

namespace FruitSalad
{
    public enum PropertyType
    {
        Float,
        Float3
    }

    public interface IPropertyMutator
    {
        void SetValue(object value);
        T GetValue<T>();
    }

    public class Property
    {
        private IPropertyMutator propertyMutator;

        public Property(string name, PropertyType type, IPropertyMutator mutator)
        {
            Name = name;
            Type = type;
            propertyMutator = mutator;
        }

        public T GetValue<T>()
        {
            return propertyMutator.GetValue<T>();
        }

        public void SetValue(object value)
        {
            propertyMutator.SetValue(value);
        }

        public string Name { get; private set; }
        public PropertyType Type { get; private set; }
    }

    public class PropertyTemplateEntry
    {
        public string Name { get; set; }
        public PropertyType Type { get; set; }
        public string DefaultValue { get; set; }
    }

    public class PropertyTemplate
    {
        public readonly LinkedList<PropertyTemplateEntry> propertyList;

        public PropertyTemplate()
        {
            propertyList = new LinkedList<PropertyTemplateEntry>();
        }

        public static PropertyTemplate FromFile(string filepath)
        {
            PropertyTemplate template = new PropertyTemplate();

            try
            {
                XmlReader reader = XmlReader.Create(filepath);
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:

                            switch (reader.Name)
                            {
                                case "Property":
                                    ReadProperty(template, reader);
                                    break;
                            }

                            break;
                    }
                }
            }
            catch(FileNotFoundException e)
            {
                throw e;
            }


            return template;
        }

        private static void ReadProperty(PropertyTemplate template, XmlReader reader)
        {
            PropertyTemplateEntry entry = new PropertyTemplateEntry()
            {
                Name = reader.GetAttribute("name"),
                Type = (PropertyType)Enum.Parse(typeof(PropertyType), reader.GetAttribute("type")),
                DefaultValue = reader.GetAttribute("default")
            };
            template.propertyList.AddLast(entry);
        }

        public IEnumerable<PropertyTemplateEntry> PropertyTemplateEntries
        {
            get
            {
                return propertyList;
            }
        }
    }

    public class PropertyContainer
    {
        private List<Property> properties;

        public PropertyContainer()
        {
            properties = new List<Property>();
        }

        public static PropertyContainer FromTemplate(PropertyTemplate template)
        {
            PropertyContainer container = new PropertyContainer();
            foreach(PropertyTemplateEntry entry in template.PropertyTemplateEntries)
            {

                IPropertyMutator mutator = null;
                switch (entry.Type)
                {
                    case PropertyType.Float:
                        mutator = new PropertyMutatorFloat();
                        break;

                    case PropertyType.Float3:
                        mutator = new PropertyMutatorFloat3();
                        break;
                }

                Property property = new Property(entry.Name, entry.Type, mutator);
                property.SetValue(entry.DefaultValue);
                container.properties.Add(property);

                Console.WriteLine(string.Format("Property(Name={0},Type={1},Value={2}", property.Name, property.Type, property.GetValue<object>()));
            }

            return container;
        }

        public IEnumerator<Property> GetEnumerator()
        {
            return properties.GetEnumerator();
        }
    }

    public class PropertyMutatorFloat : IPropertyMutator
    {
        private object myValue;

        public T GetValue<T>()
        {
            return (T)myValue;
        }

        public void SetValue(object value)
        {
            if (value.GetType() == typeof(float))
            {
                myValue = value;
            }
            else if(value.GetType() == typeof(string))
            {
                myValue = float.Parse((string)value, CultureInfo.InvariantCulture.NumberFormat);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }
    }

    public class PropertyMutatorFloat3 : IPropertyMutator
    {
        private object myValue;

        public T GetValue<T>()
        {
            return (T)myValue;
        }

        public void SetValue(object value)
        {
            if (value.GetType() == typeof(Float3))
            {
                myValue = value;
            }
            else if (value.GetType() == typeof(string))
            {
                myValue = Float3.Parse((string) value);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }
    }
}
