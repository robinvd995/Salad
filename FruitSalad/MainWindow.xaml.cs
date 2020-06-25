using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using FruitSalad.Director;
using FruitSalad.Util;
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

            Logger.SetLogger(new TextBoxLogger(ConsoleBox));

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
            openFileDialog.Filter = Constants.FruitSaladWorkspaceFilter;
            if (openFileDialog.ShowDialog() == true)
            {
                String filename = openFileDialog.FileName;
                FruitSaladApp.Instance.LoadWorkspace(filename);
            }
        }

        private void MenuSaveClick(object sender, RoutedEventArgs e)
        {
            FruitSaladApp.Instance.SaveFile();
        }
        private void MenuExitClick(object sender, RoutedEventArgs e)
        {
            FruitSaladApp.Instance.Exit();
        }

        private void TreeItemDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (sender is DirectoryTreeViewItem)
            {
                if (!((DirectoryTreeViewItem)sender).IsSelected)
                {
                    return;
                }

                DirectoryTreeViewItem item = (DirectoryTreeViewItem)sender;
                string filePath = item.FilePath;
                if (filePath.Length > 0)
                {
                    FSFile file = new FSFile()
                    {
                        AbosultePath = filePath,
                        Extension = item.FileType
                    };

                    FruitSaladApp.Instance.OpenFile(file);
                }
            }
        }

        private void FloatValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex(@"^[0-9]*(?:\.[0-9]*)?$");
            e.Handled = !regex.IsMatch(e.Text);
        }

        private void CommandBindingSaveFile(object sender, ExecutedRoutedEventArgs e)
        {
            Console.WriteLine("Saving!!!");
            FruitSaladApp.Instance.SaveFile();
        }

        private void BuildWorkspaceClick(object sender, RoutedEventArgs e)
        {
            SharpGLWindow sharpGLWindow = new SharpGLWindow();
            sharpGLWindow.ShowDialog();
        }
    }

    public class DirectorVM : ViewModelBase
    {
        private List<DirectorItemVM> items = new List<DirectorItemVM>();

        public DirectorVM()
        {

        }

        public List<DirectorItemVM> DirectorItems
        {
            get
            {
                return items;
            }
        }

        public void LoadFromTree(Tree<DirectorItemData> tree)
        {
            DirectorItemVM root = AddNode(tree.RootNode);
            items = new List<DirectorItemVM>() { root };
            OnPropertyChanged("DirectorItems");
        }

        private DirectorItemVM AddNode(TreeNode<DirectorItemData> node)
        {
            DirectorItemVM vmnode = new DirectorItemVM(node.NodeData);
            foreach (TreeNode<DirectorItemData> child in node.Children)
            {
                vmnode.Children.Add(AddNode(child));
            }

            return vmnode;
        }
    }

    public class MainWindowVM : ViewModelBase
    {
        //private List<DirectorItemVM> list = new List<DirectorItemVM>();
        private DirectorVM directorVM = new DirectorVM();
        private List<PropertyVM> properties = new List<PropertyVM>();
        private SourceContainerVM sourceVM;
        //private EditorVM editorVM;

        public MainWindowVM()
        {
        }

        public void SetDirectoryTree(Tree<DirectorItemData> tree)
        {
            directorVM.LoadFromTree(tree);
            OnPropertyChanged("DirectoryTree");
        }

        public DirectorVM DirectoryTree
        {
            get
            {
                return directorVM;
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

        public SourceContainerVM SourceData
        {
            get
            {
                return sourceVM;
            }
            set
            {
                sourceVM = value;
                OnPropertyChanged("SourceData");
            }
        }

        public void LoadFile(FileContainer fileContainer)
        {
            LoadProperties(fileContainer.Properties);
            SourceData = fileContainer.Source.GetViewModel();
        }

        private void LoadProperties(PropertyContainer container)
        {
            if (container == null)
            {
                Properties = new List<PropertyVM>();
            }
            else
            {
                List<PropertyVM> propertyList = new List<PropertyVM>();
                IEnumerator<Property> it = container.GetEnumerator();
                while (it.MoveNext())
                {
                    Property property = it.Current;
                    PropertyVM propertyVM = property.Type.CreatePropertyViewModel(property);
                    propertyList.Add(propertyVM);
                }

                Properties = propertyList;
            }
        }
    }

    public class EditorTemplateSelector : DataTemplateSelector
    {
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            FrameworkElement element = container as FrameworkElement;

            if (element != null && item != null && item is SourceContainerVM)
            {
                SourceContainerVM source = item as SourceContainerVM;
                string fileTypeName = source.GetFileType().Name.FirstCharToUpper();
                try
                {
                    return element.FindResource("EditorTemplate" + fileTypeName) as DataTemplate;
                }
                catch(Exception)
                {
                    return null;
                }
            }

            return null;
        }
    }

    public class TextBoxLogger : ILogger
    {
        private readonly TextBox logTextBox;

        public TextBoxLogger(TextBox textBox)
        {
            logTextBox = textBox;
        }

        public void Log(string s)
        {
            logTextBox.AppendText(s + Environment.NewLine);
            logTextBox.ScrollToEnd();
        }
    }
}
