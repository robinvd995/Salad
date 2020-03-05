using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml;

namespace FruitSalad
{
    class FruitSaladApp
    {
        // -------- Singleton Start --------
        private static FruitSaladApp instance = null;
        private static readonly object instanceLock = new object();

        public static FruitSaladApp Instance
        {
            get
            {
                lock (instanceLock)
                {
                    if (instance == null)
                        instance = new FruitSaladApp();
                    return instance;
                }
            }
        }
        // -------- Singleton End --------

        public Workspace WorkspaceObj { get; private set; }
        public void LoadWorkspace(String filepath)
        {
            WorkspaceObj = Workspace.GetWorkspace(filepath);
            MainWindow.Instance.ViewModel.DirectorItems = CreateDirectoryTree();
            LoadPropertyTemplates();
        }

        public void CreateNewWorkspace(string workspaceName, string directory, string sourceDir, string propertyDir, string outputDir)
        {
            string file = Workspace.CreateNewWorkspace(workspaceName, directory, sourceDir, propertyDir, outputDir);
            LoadWorkspace(file);
        }

        public void OpenFile(string filePath)
        {
            string extension = Path.GetExtension(filePath);
            Console.WriteLine(extension);
            FileType type = Enumeration.GetAll<FileType>().First(ft => ft.Extension == extension);
            if(type == null)
            {
                Console.WriteLine("Invalid file extension!");
                return;
            }

            if (type.IsProperty)
            {
                PropertyContainer container = PropertyContainer.FromTemplate(type.Template);
                MainWindow.Instance.ViewModel.LoadProperties(container);
            }
        }

        private void LoadPropertyTemplates()
        {
            string dirPath = WorkspaceObj.WorkingDirectory + Constants.PropertyTemplateFolder;
            foreach(FileType type in Enumeration.GetAll<FileType>())
            {
                if (type.IsProperty)
                {
                    string fileName = dirPath + Path.DirectorySeparatorChar + type.Name + Constants.PropertyTemplateFileExtension;
                    try
                    {
                        type.Template = PropertyTemplate.FromFile(fileName);
                        Console.WriteLine(string.Format("Loaded template for type '{0}'", type.Name));
                    }
                    catch(FileNotFoundException)
                    {
                        Console.WriteLine(string.Format("Could not load template for type '{0}', filepath={1}", type.Name, fileName));
                    }
                }
            }
        }

        private List<DirectorItem> CreateDirectoryTree()
        {
            List<DirectorItem> itemList = new List<DirectorItem>();
            DirectorItem rootItem = new DirectorItem()
            {
                ItemName = string.Format("Workspace '{0}'", WorkspaceObj.Name),
                ItemType = FileType.Workspace
            };

            DirectorItem sourceItem = new DirectorItem()
            {
                ItemName = "Source",
                ItemType = FileType.SourceFolder
            };
            AddDirectory(sourceItem, WorkspaceObj.SourceDirectory);
            rootItem.Children.Add(sourceItem);

            DirectorItem propertiesItem = new DirectorItem()
            {
                ItemName = "Properties",
                ItemType = FileType.PropertiesFolder
            };
            AddDirectory(propertiesItem, WorkspaceObj.PropertiesDirectory);
            rootItem.Children.Add(propertiesItem);

            DirectorItem outputItem = new DirectorItem()
            {
                ItemName = "Output",
                ItemType = FileType.OuputFolder
            };
            AddDirectory(outputItem, WorkspaceObj.OutputDirectory);
            rootItem.Children.Add(outputItem);

            itemList.Add(rootItem);

            return itemList;
        }

        private void AddDirectory(DirectorItem item, string directory)
        { 
            string[] directories = Directory.GetDirectories(directory);
            foreach(string s in directories)
            {
                string[] folders = s.Split(Path.DirectorySeparatorChar);
                string dirName = folders[folders.Length - 1];
                DirectorItem directorItem = new DirectorItem()
                {
                    ItemName = dirName,
                    ItemType = FileType.Folder
                };
                item.Children.Add(directorItem);
                AddDirectory(directorItem, s);
            }

            string[] files = Directory.GetFiles(directory);
            foreach (string s in files)
            {
                string name = Path.GetFileName(s);
                string extension = Path.GetExtension(s);
                IEnumerable<FileType> matchingTypes = Enumeration.GetAll<FileType>().Where(dit => dit.Extension == extension);
                FileType type = FileType.Unknown;
                if(matchingTypes.Count() == 1)
                {
                    type = matchingTypes.First();
                }

                DirectorItem directorItem = new DirectorItem()
                {
                    ItemName = name,
                    ItemType = type,
                    AbsolutePath = s
                };

                item.Children.Add(directorItem);
            }
        }
    }
}
