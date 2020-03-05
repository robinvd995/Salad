using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.Win32;


namespace FruitSalad
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly MainWindowVM viewModel;

        public MainWindow()
        {
            InitializeComponent();

            viewModel = new MainWindowVM();
            DataContext = viewModel;

            Instance = this;
        }

        public static MainWindow Instance { get; private set; }

        public MainWindowVM ViewModel { get { return viewModel; } }

        private void MenuNewClick(object sender, RoutedEventArgs e)
        {
            NewWorkspaceWindow newWorkspaceWindow = new NewWorkspaceWindow();
            newWorkspaceWindow.ShowDialog();
        }

        private void MenuOpenClick(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Fruit Salad Workspace (*.fsws)|*.fsws|All Files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == true)
            {
                String filename = openFileDialog.FileName;
                FruitSaladApp.Instance.LoadWorkspace(filename);
            }
        }

        private void TreeItemDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (sender is TreeViewItem)
            {
                if (!((TreeViewItem)sender).IsSelected)
                {
                    return;
                }

                TreeViewItem item = (TreeViewItem)sender;
                string filePath = item.Tag.ToString();
                if(filePath.Length > 0)
                {
                    FruitSaladApp.Instance.OpenFile(filePath);
                }
            }
        }

        private void FloatValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex(@"^[0-9]*(?:\.[0-9]*)?$");
            e.Handled = !regex.IsMatch(e.Text);
        }
    }

    public class MainWindowVM : ViewModelBase
    {
        private List<DirectorItem> list = new List<DirectorItem>();
        private List<PropertyVM> properties = new List<PropertyVM>();

        public MainWindowVM()
        {
        }

        public List<DirectorItem> DirectorItems
        {
            get
            {
                return list;
            }
            set
            {
                list = value;
                OnPropertyChanged("DirectorItems");
            }
        }

        public List<PropertyVM> Properties
        {
            get
            {
                return properties;
            }
            set
            {
                properties = value;
                OnPropertyChanged("Properties");
            }
        }

        public void LoadProperties(PropertyContainer container)
        {
            List<PropertyVM> propertyList = new List<PropertyVM>();
            Console.WriteLine("Loading Properties");
            IEnumerator<Property> it = container.GetEnumerator();
            while (it.MoveNext())
            {
                Property property = it.Current;
                PropertyVM propertyVM = null;
                switch (property.Type)
                {
                    case PropertyType.Float:
                        Console.WriteLine("Loading Float");
                        propertyVM = new PropertyFloatVM(property);
                        break;

                    case PropertyType.Float3:
                        Console.WriteLine("Loading Float3");
                        propertyVM = new PropertyFloat3VM(property);
                        break;
                }

                propertyList.Add(propertyVM);
            }

            Properties = propertyList;
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
    }

    public class PropertyFloatVM : PropertyVM
    {

        public PropertyFloatVM(Property property) :
            base(property)
        {}

        public string Value
        {
            get
            {
                return propertyObj.GetValue<float>().ToString();
            }
            set
            {
                propertyObj.SetValue(value);
                OnPropertyChanged("Value");
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
                return value.X.ToString();
            }
            set
            {
                float fvalue = float.Parse(value, CultureInfo.InvariantCulture.NumberFormat);
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
                return value.Y.ToString();
            }
            set
            {
                float fvalue = float.Parse(value, CultureInfo.InvariantCulture.NumberFormat);
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
                return value.Z.ToString();
            }
            set
            {
                float fvalue = float.Parse(value, CultureInfo.InvariantCulture.NumberFormat);
                Float3 float3 = propertyObj.GetValue<Float3>();
                float3.Z = fvalue;
                OnPropertyChanged("ValueZ");
            }
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

                if(property.Type == PropertyType.Float)
                {
                    return element.FindResource("PropertyTemplateFloat") as DataTemplate;
                }
                if (property.Type == PropertyType.Float3)
                {
                    return element.FindResource("PropertyTemplateFloat3") as DataTemplate;
                }
                else
                {
                    return element.FindResource("PropertyTemplateUnknown") as DataTemplate;
                }
            }

            return null;
        }
    }
}
