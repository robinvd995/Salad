using FruitSalad.Director;
using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
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

        private readonly IDialogService dialogService = new DialogService();

        //private PropertyContainer currentPropertyContainer = null;
        private FileContainer currentOpenFile = null;
        private Tree<DirectorItemData> directoryTree;

        public Workspace WorkspaceObj { get; private set; }
        public void LoadWorkspace(string filepath)
        {
            WorkspaceObj = Workspace.GetWorkspace(filepath);
            directoryTree = DirectorTree.CreateDirectoryTree(WorkspaceObj);
            MainWindow.Instance.ViewModel.SetDirectoryTree(directoryTree);
            LoadPropertyTemplates();
        }

        public void CreateNewWorkspace(string workspaceName, string directory, string sourceDir, string propertyDir, string outputDir, string assetsDir)
        {
            string file = Workspace.CreateNewWorkspace(workspaceName, directory, sourceDir, propertyDir, outputDir, assetsDir);
            LoadWorkspace(file);
        }

        public void OpenFile(FileContainer file)
        {
            currentOpenFile = file;
            MainWindow.Instance.ViewModel.LoadFile(currentOpenFile);
        }

        public void OpenFile(FSFile file)
        {
            Logger.LogMessage(file.AbosultePath);

            try
            {
                FileType type = Enumeration.GetAll<FileType>().Single(ft => ft.Name.Equals(file.Extension));
                /*IFileTypeHandler handler = Enumeration.GetAll<FileType>().Single(ft => ft.Name.Equals(file.Extension)).FileTypeHandler;

                currentOpenFile = handler.OpenFile(file);
                MainWindow.Instance.ViewModel.LoadFile(currentOpenFile);*/
            }
            catch (Exception)
            {
                Logger.ErrorMessage(string.Format("Error opening file '{0}'", file.AbosultePath));
            }

            /*string extension = Path.GetExtension(filePath);
            FileType type = Enumeration.GetAll<FileType>().First(ft => ft.Extension == extension);
            if(type == null)
            {
                Console.WriteLine("Invalid file extension!");
                return;
            }

            FileContainer fileContainer = new FileContainer();

            if (type.HasProperties && type.IsSourceFile)
            {
                int filePrefixLength = WorkspaceObj.SourceDirectory.Length;
                string relativeFilePath = filePath.Substring(filePrefixLength);
                string propertyPath = WorkspaceObj.PropertiesDirectory + relativeFilePath + FileType.Properties.Extension;
                Console.WriteLine(propertyPath);
                bool exists = File.Exists(propertyPath);
                if (!exists)
                {
                    var dialog = new DialogVMYesNo("Create new properties file", "Could not find an existing properties file.\nDo you want to create a new one?");
                    DialogResult result = dialogService.OpenDialog(dialog);
                    if(result == DialogResult.Yes)
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

            if (type.CanEdit)
            {
                MainWindow.Instance.ViewModel.EditorData = new EditorVM(type, filePath);
            }

            MainWindow.Instance.ViewModel.LoadFile(fileContainer);
            currentOpenFile = fileContainer;*/
        }

        public void SaveFile()
        {
            /*if(currentOpenFile == null || currentOpenFile.PropertyFilePath.Length == 0)
            {
                var dialog = new DialogVMAlert("Error", "No property file path is set!");
                dialogService.OpenDialog(dialog);
                return;
            }

            if (!File.Exists(currentOpenFile.PropertyFilePath))
            {
                var dialog = new DialogVMAlert("Error", "Trying to save to an property file that does not exists!");
                dialogService.OpenDialog(dialog);
                return;
            }*/

            if (currentOpenFile == null)
            {
                Logger.ErrorMessage("Error while saving current document!");
                return;
            }

            currentOpenFile.SaveFile();
        }

        public void Exit()
        {
            var dialog = new DialogVMYesNo("Exit Application", "Are you sure you want to exit?");
            DialogResult result = dialogService.OpenDialog(dialog);
            if (result == DialogResult.Yes)
            {
                MainWindow.Instance.Close();
            }
        }

        private void LoadPropertyTemplates()
        {
            string dirPath = WorkspaceObj.WorkingDirectory + Constants.PropertyTemplateFolder;
            foreach (FileType type in Enumeration.GetAll<FileType>())
            {
                if (type.HasProperties)
                {
                    string fileName = dirPath + Path.DirectorySeparatorChar + type.Name + Constants.PropertyTemplateFileExtension;
                    try
                    {
                        type.Template = PropertyTemplate.FromFile(fileName);
                        Console.WriteLine(string.Format("Loaded template for type '{0}'", type.Name));
                    }
                    catch (FileNotFoundException)
                    {
                        Console.WriteLine(string.Format("Could not load template for type '{0}', filepath={1}", type.Name, fileName));
                    }
                }
            }
        }

        public IDialogService GetDialogService()
        {
            return dialogService;
        }
    }
}
