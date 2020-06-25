using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;

namespace FruitSalad
{
    public class PropertyType : Enumeration
    {
        public static PropertyType Float  = new PropertyType(0, "Float",  typeof(PropertyMutatorFloat ), typeof(PropertyFloatVM ));
        public static PropertyType Float2 = new PropertyType(1, "Float2", typeof(PropertyMutatorFloat2), typeof(PropertyFloat2VM));
        public static PropertyType Float3 = new PropertyType(2, "Float3", typeof(PropertyMutatorFloat3), typeof(PropertyFloat3VM));
        public static PropertyType Float4 = new PropertyType(3, "Float4", typeof(PropertyMutatorFloat4), typeof(PropertyFloat4VM));

        private readonly Type propertyMutatorType;
        private readonly Type propertyViewModelType;

        public PropertyType(int id, string name, Type mutator, Type propertyVM) :
            base(id, name)
        {
            propertyMutatorType = mutator;
            propertyViewModelType = propertyVM;
        }

        public IPropertyMutator CreatePropertyMutator()
        {
            object mutator = Activator.CreateInstance(propertyMutatorType);
            if(mutator is IPropertyMutator)
            {
                return (IPropertyMutator)Activator.CreateInstance(propertyMutatorType);
            }
            else
            {
                throw new Exception("PropertyMutator type error!");
            }
        }

        public PropertyVM CreatePropertyViewModel(Property property)
        {
            var constructor = propertyViewModelType.GetConstructor(new Type[] { typeof(Property) });
            object propertyVM = constructor.Invoke(new object[] { property });
            if(propertyVM is PropertyVM)
            {
                return (PropertyVM)propertyVM;
            }
            else
            {
                throw new Exception("PropertyViewModel type error!");
            }
        }
    }

    public interface IPropertyMutator
    {
        void SetValue(object value);
        T GetValue<T>();

        string SerializeProperty();
        void DeserializeProperty(string value);
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

        public string SerializeValue()
        {
            return propertyMutator.SerializeProperty();
        }

        public void DeserializeValue(string value)
        {
            propertyMutator.DeserializeProperty(value);
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
                Type = Enumeration.GetAll<PropertyType>().Single(pt => pt.Name == reader.GetAttribute("type")),
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
        public static readonly string PropertyElementKeyRoot = "Properties";
        public static readonly string PropertyElementKeyProperty = "Property";

        public static readonly string PropertyAttribKeyName = "name";

        private List<Property> properties;

        public PropertyContainer()
        {
            properties = new List<Property>();
        }

        public string PropertyFilePath { get; private set; }

        public static PropertyContainer FromTemplate(PropertyTemplate template)
        {
            PropertyContainer container = new PropertyContainer();
            foreach(PropertyTemplateEntry entry in template.PropertyTemplateEntries)
            {
                IPropertyMutator mutator = entry.Type.CreatePropertyMutator();

                Property property = new Property(entry.Name, entry.Type, mutator);
                property.SetValue(entry.DefaultValue);
                container.properties.Add(property);

                Console.WriteLine(string.Format("Property(Name={0},Type={1},Value={2}", property.Name, property.Type, property.GetValue<object>()));
            }

            return container;
        }

        public void LoadPropertiesFromFile(string propertiesFile)
        {
            XmlReader reader = XmlReader.Create(propertiesFile);

            PropertyFilePath = propertiesFile;

            bool isProperty = false;
            string propertyName = "";
            string propertyValue = "";

            try
            {
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            if (reader.Name == PropertyElementKeyProperty)
                            {
                                if (!isProperty)
                                {
                                    isProperty = true;
                                    propertyName = reader.GetAttribute(PropertyAttribKeyName);
                                }
                                else
                                {
                                    Console.WriteLine("ERROR!");
                                }
                            }
                            break;

                        case XmlNodeType.Text:
                            propertyValue = reader.Value;
                            break;

                        case XmlNodeType.EndElement:
                            if (isProperty)
                            {
                                Property property = FindProperty(propertyName);
                                if (property == null)
                                {
                                    Console.WriteLine("Could not find property!!!!");
                                }
                                else
                                {
                                    property.DeserializeValue(propertyValue);
                                }
                                propertyName = "";
                                propertyValue = "";
                                isProperty = false;
                            }
                            break;
                    }
                }
            }
            catch(Exception)
            {
                Console.WriteLine("Error occured while loading properties file!");
            }

            reader.Close();
        }

        public void SavePropertiesToFile()
        {
            XmlWriterSettings xmlWriterSettings = new XmlWriterSettings()
            {
                Indent = true,
                IndentChars = "    "
            };

            Console.WriteLine(PropertyFilePath);

            XmlWriter writer = XmlWriter.Create(PropertyFilePath, xmlWriterSettings);
            writer.WriteStartElement(PropertyElementKeyRoot);
            List<PropertyVM> propertyList = MainWindow.Instance.ViewModel.Properties;
            foreach (PropertyVM pvm in propertyList)
            {
                Property property = pvm.GetProperty();
                string serializedPropertyValue = property.SerializeValue();
                string propertyString = string.Format("Property.{0}({1}) : {2}", property.Name, serializedPropertyValue, property.Type.Name);

                writer.WriteStartElement(PropertyElementKeyProperty);
                writer.WriteAttributeString(PropertyAttribKeyName, property.Name);
                writer.WriteValue(serializedPropertyValue);
                writer.WriteEndElement();
            }
            writer.WriteEndElement();

            writer.Flush();
            writer.Close();
        }

        public Property FindProperty(string name)
        {
            IEnumerator<Property> enumerator = GetEnumerator();
            while (enumerator.MoveNext())
            {
                if (enumerator.Current.Name == name)
                    return enumerator.Current;
            }

            return null;
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
                DeserializeProperty((string)value);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }

        public string SerializeProperty()
        {
            return Parser.FloatToString((float) myValue);
        }
        public void DeserializeProperty(string value)
        {
            myValue = Parser.ParseFloat(value);
        }
    }

    public class PropertyMutatorFloat2 : IPropertyMutator
    {
        private object myValue;

        public T GetValue<T>()
        {
            return (T)myValue;
        }

        public void SetValue(object value)
        {
            if (value.GetType() == typeof(Float2))
            {
                myValue = value;
            }
            else if (value.GetType() == typeof(string))
            {
                DeserializeProperty((string)value);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }

        public string SerializeProperty()
        {
            return ((Float2)myValue).Serialize();
        }

        public void DeserializeProperty(string value)
        {
            myValue = Float2.Parse(value);
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
                DeserializeProperty((string)value);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }

        public string SerializeProperty()
        {
            return ((Float3)myValue).Serialize();
        }
        public void DeserializeProperty(string value)
        {
            myValue = Float3.Parse(value);
        }
    }

    public class PropertyMutatorFloat4 : IPropertyMutator
    {
        private object myValue;

        public T GetValue<T>()
        {
            return (T)myValue;
        }

        public void SetValue(object value)
        {
            if (value.GetType() == typeof(Float4))
            {
                myValue = value;
            }
            else if (value.GetType() == typeof(string))
            {
                DeserializeProperty((string)value);
            }
            else
            {
                Console.WriteLine("Trying to set value that is not compatible with this Property!");
            }
        }

        public string SerializeProperty()
        {
            return ((Float4)myValue).Serialize();
        }
        public void DeserializeProperty(string value)
        {
            myValue = Float4.Parse((string)value);
        }
    }

    public class PropertyTemplateSelector : DataTemplateSelector
    {
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            FrameworkElement element = container as FrameworkElement;

            if (element != null && item != null && item is PropertyVM)
            {
                PropertyVM property = item as PropertyVM;
                return element.FindResource("PropertyTemplate" + property.Type.Name) as DataTemplate;
            }

            return null;
        }
    }

    public class PropertyVM : ViewModelBase
    {
        protected Property propertyObj;

        public PropertyVM(Property property)
        {
            propertyObj = property;
        }

        public string Name
        {
            get
            {
                return propertyObj.Name;
            }
            set
            {
                throw new Exception("Can not edit the name of a Property!");
            }
        }

        public PropertyType Type
        {
            get
            {
                return propertyObj.Type;
            }
            set
            {
                throw new Exception("Can not edit the type of a Property!");
            }
        }

        public Property GetProperty()
        {
            return propertyObj;
        }
    }

    public class PropertyFloatVM : PropertyVM
    {

        public PropertyFloatVM(Property property) :
            base(property)
        { }

        public string Value
        {
            get
            {
                return Parser.FloatToString(propertyObj.GetValue<float>());
            }
            set
            {
                propertyObj.SetValue(value);
                OnPropertyChanged("Value");
            }
        }
    }

    public class PropertyFloat2VM : PropertyVM
    {
        public PropertyFloat2VM(Property property) :
            base(property)
        {
        }

        public string ValueX
        {
            get
            {
                Float2 value = propertyObj.GetValue<Float2>();
                return Parser.FloatToString(value.X);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float2 float2 = propertyObj.GetValue<Float2>();
                float2.X = fvalue;
                OnPropertyChanged("ValueX");
            }
        }

        public string ValueY
        {
            get
            {
                Float2 value = propertyObj.GetValue<Float2>();
                return Parser.FloatToString(value.Y);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float2 float2 = propertyObj.GetValue<Float2>();
                float2.Y = fvalue;
                OnPropertyChanged("ValueY");
            }
        }
    }

    public class PropertyFloat3VM : PropertyVM
    {
        public PropertyFloat3VM(Property property) :
            base(property)
        {
        }

        public string ValueX
        {
            get
            {
                Float3 value = propertyObj.GetValue<Float3>();
                return Parser.FloatToString(value.X);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float3 float3 = propertyObj.GetValue<Float3>();
                float3.X = fvalue;
                OnPropertyChanged("ValueX");
            }
        }

        public string ValueY
        {
            get
            {
                Float3 value = propertyObj.GetValue<Float3>();
                return Parser.FloatToString(value.Y);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float3 float3 = propertyObj.GetValue<Float3>();
                float3.Y = fvalue;
                OnPropertyChanged("ValueY");
            }
        }

        public string ValueZ
        {
            get
            {
                Float3 value = propertyObj.GetValue<Float3>();
                return Parser.FloatToString(value.Z);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float3 float3 = propertyObj.GetValue<Float3>();
                float3.Z = fvalue;
                OnPropertyChanged("ValueZ");
            }
        }
    }

    public class PropertyFloat4VM : PropertyVM
    {
        public PropertyFloat4VM(Property property) :
            base(property)
        {
        }

        public string ValueX
        {
            get
            {
                Float4 value = propertyObj.GetValue<Float4>();
                return Parser.FloatToString(value.X);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float4 float4 = propertyObj.GetValue<Float4>();
                float4.X = fvalue;
                OnPropertyChanged("ValueX");
            }
        }

        public string ValueY
        {
            get
            {
                Float4 value = propertyObj.GetValue<Float4>();
                return Parser.FloatToString(value.Y);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float4 float4 = propertyObj.GetValue<Float4>();
                float4.Y = fvalue;
                OnPropertyChanged("ValueY");
            }
        }

        public string ValueZ
        {
            get
            {
                Float4 value = propertyObj.GetValue<Float4>();
                return Parser.FloatToString(value.Z);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float4 float4 = propertyObj.GetValue<Float4>();
                float4.Z = fvalue;
                OnPropertyChanged("ValueZ");
            }
        }

        public string ValueW
        {
            get
            {
                Float4 value = propertyObj.GetValue<Float4>();
                return Parser.FloatToString(value.W);
            }
            set
            {
                float fvalue = Parser.ParseFloat(value);
                Float4 float4 = propertyObj.GetValue<Float4>();
                float4.W = fvalue;
                OnPropertyChanged("ValueW");
            }
        }
    }
}
