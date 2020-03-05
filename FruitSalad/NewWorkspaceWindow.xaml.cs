using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.IO;

namespace FruitSalad
{
    /// <summary>
    /// Interaction logic for NewWorkspaceWindow.xaml
    /// </summary>
    public partial class NewWorkspaceWindow : Window
    {
        private readonly NewWorkspaceVM viewModel;
        public NewWorkspaceWindow()
        {
            InitializeComponent();
            viewModel = new NewWorkspaceVM();
            DataContext = viewModel;
        }

        private void ButtonWorkspace(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            var result = folderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                string rootDirectory = folderDialog.SelectedPath;
                viewModel.WorkspaceRoot = rootDirectory;
            }
        }

        private void ButtonSource(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            var result = folderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                string directory = folderDialog.SelectedPath + System.IO.Path.DirectorySeparatorChar;
                viewModel.SourceDirectory = directory;
            }
        }

        private void ButtonProperty(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            var result = folderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                string directory = folderDialog.SelectedPath + System.IO.Path.DirectorySeparatorChar;
                viewModel.PropertyDirectory = directory;
            }
        }

        private void ButtonOutput(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            var result = folderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                string directory = folderDialog.SelectedPath + System.IO.Path.DirectorySeparatorChar;
                viewModel.OutputDirectory = directory;
            }
        }

        private void ButtonCreate(object sender, RoutedEventArgs e)
        {
            FruitSaladApp.Instance.CreateNewWorkspace(viewModel.WorkspaceName, viewModel.WorkspaceDirectory, viewModel.SourceDirectory, viewModel.PropertyDirectory, viewModel.OutputDirectory);
            Close();
        }

        private void ButtonReset(object sender, RoutedEventArgs e)
        {
            viewModel.WorkspaceName = "";
            viewModel.WorkspaceRoot = "";
            viewModel.WorkspaceDirectory = "";
            viewModel.HasSourceDir = false;
            viewModel.HasPropertyDir = false;
            viewModel.HasOutputDir = false;
            viewModel.SourceDirectory = "";
            viewModel.PropertyDirectory = "";
            viewModel.OutputDirectory = "";
        }

        private void ButtonCancel(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }

    public class NewWorkspaceVM : ViewModelBase
    {
        private string workspaceName;
        private string workspaceRoot;
        private string workspaceDirectory;

        private string customSourceDirectory = "";
        private string customPropertyDirectory = "";
        private string customOutputDirectory = "";

        private bool hasCustomSourceDir = false;
        private bool hasCustomPropertyDir = false;
        private bool hasCustomOutputDir = false;

        public string WorkspaceName
        {
            get
            {
                return workspaceName;
            }
            set
            {
                workspaceName = value;
                char separator = System.IO.Path.DirectorySeparatorChar;
                WorkspaceDirectory = workspaceRoot + separator + workspaceName + separator;
                OnPropertyChanged("WorkspaceName");
            }
        }

        public string WorkspaceRoot
        {
            get
            {
                return workspaceRoot;
            }
            set
            {
                workspaceRoot = value;
                char separator = System.IO.Path.DirectorySeparatorChar;
                WorkspaceDirectory = workspaceRoot + separator + workspaceName + separator;
            }
        }

        public string WorkspaceDirectory
        {
            get
            {
                return workspaceDirectory;
            }
            set
            {
                char separator = System.IO.Path.DirectorySeparatorChar;
                workspaceDirectory = value;

                OnPropertyChanged("WorkspaceDirectory");
                OnPropertyChanged("SourceDirectory");
                OnPropertyChanged("PropertyDirectory");
                OnPropertyChanged("OutputDirectory");
            }
        }

        public string SourceDirectory
        {
            get
            {
                char separator = System.IO.Path.DirectorySeparatorChar;
                return hasCustomSourceDir ? customSourceDirectory : workspaceDirectory + "source" + separator;
            }
            set
            {
                customSourceDirectory = value;
                OnPropertyChanged("SourceDirectory");
            }
        }

        public string PropertyDirectory
        {
            get
            {
                char separator = System.IO.Path.DirectorySeparatorChar;
                return hasCustomPropertyDir ? customPropertyDirectory : workspaceDirectory + "properties" + separator;
            }
            set
            {
                customPropertyDirectory = value;
                OnPropertyChanged("PropertyDirectory");
            }
        }

        public string OutputDirectory
        {
            get
            {
                char separator = System.IO.Path.DirectorySeparatorChar;
                return hasCustomOutputDir ? customOutputDirectory : workspaceDirectory + "output" + separator;
            }
            set
            {
                customOutputDirectory = value;
                OnPropertyChanged("OutputDirectory");
            }
        }

        public bool HasSourceDir
        {
            get
            {
                return hasCustomSourceDir;
            }
            set
            {
                hasCustomSourceDir = value;
                OnPropertyChanged("HasSourceDir");
                OnPropertyChanged("IsSourceReadOnly");
                OnPropertyChanged("SourceDirectory");
            }
        }

        public bool IsSourceReadOnly { get {  return !HasSourceDir; } }

        public bool HasPropertyDir
        {
            get
            {
                return hasCustomPropertyDir;
            }
            set
            {
                hasCustomPropertyDir = value;
                OnPropertyChanged("HasPropertyDir");
                OnPropertyChanged("IsPropertyReadOnly");
                OnPropertyChanged("PropertyDirectory");
            }
        }

        public bool IsPropertyReadOnly { get { return !HasPropertyDir; } }

        public bool HasOutputDir
        {
            get
            {
                return hasCustomOutputDir;
            }
            set
            {
                hasCustomOutputDir = value;
                OnPropertyChanged("HasOutputDir");
                OnPropertyChanged("IsOutputReadOnly");
                OnPropertyChanged("OutputDirectory");
            }
        }

        public bool IsOutputReadOnly { get { return !HasOutputDir; } }
    }
}
