using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Director
{
    public class DirectorTree
    {
        public static Tree<DirectorItemData> CreateDirectoryTree(Workspace workspace)
        {
            Tree<DirectorItemData> tree = new Tree<DirectorItemData>(new DirectorItemData(workspace.WorkingDirectory, "Workspace", DirectoryTreeType.Workspace));

            DirectoryTreePopulator assetsPopulator = new DirectoryTreePopulator(workspace.AssetsDirectory, "Assets", DirectoryTreeType.AssetsFolder);
            DirectoryTreePopulator sourcePopulator = new DirectoryTreePopulator(workspace.SourceDirectory, "Source", DirectoryTreeType.SourceFolder);
            DirectoryTreePopulator propertiesPopulator = new DirectoryTreePopulator(workspace.PropertiesDirectory, "Properties", DirectoryTreeType.PropertiesFolder);
            DirectoryTreePopulator outputPopulator = new DirectoryTreePopulator(workspace.OutputDirectory, "Output", DirectoryTreeType.OutputFolder);

            tree.AddSubBranch(assetsPopulator);
            tree.AddSubBranch(sourcePopulator);
            tree.AddSubBranch(propertiesPopulator);
            tree.AddSubBranch(outputPopulator);

            return tree;
        }
    }

    internal class DirectoryTreePopulator : ITreePopulator<DirectorItemData>
    {
        public string Dir { get; set; }
        public TreeNode<DirectorItemData> Root;

        public DirectoryTreePopulator(string directory, string name, DirectoryTreeType type)
        {
            Dir = directory;
            Root = new TreeNode<DirectorItemData>(new DirectorItemData(directory, type.Name, type));
        }

        public TreeNode<DirectorItemData> Populate()
        {
            PopulateNode(Root);
            return Root;
        }

        private void PopulateNode(TreeNode<DirectorItemData> node)
        {
            string dir = node.NodeData.AbsolutePath;

            string[] directories = Directory.GetDirectories(dir);
            foreach (string subdir in directories)
            {
                string[] folders = subdir.Split(Path.DirectorySeparatorChar);
                string dirName = folders[folders.Length - 1];
                DirectorItemData directorData = new DirectorItemData(subdir, dirName, DirectoryTreeType.Folder);
                PopulateNode(node.AddChild(directorData));
            }

            string[] files = Directory.GetFiles(dir);
            foreach (string file in files)
            {
                string name = Path.GetFileName(file);
                string extension = Path.GetExtension(file);
                IEnumerable<FileType> matchingTypes = Enumeration.GetAll<FileType>().Where(ft => ft.Extension == extension);
                FileType type = FileType.Unknown;
                if (matchingTypes.Count() == 1)
                {
                    type = matchingTypes.First();
                }
                else
                {
                    Logger.ErrorMessage("Multipile file types found for file!");
                }

                DirectoryTreeType treeType = GetDirectoryTreeTypeFromFileType(type);
                DirectorItemData directorData = new DirectorItemData(file, name, treeType);
                node.AddChild(directorData);
            }
        }

        private DirectoryTreeType GetDirectoryTreeTypeFromFileType(FileType type)
        {
            var x = Enumeration.GetAll<DirectoryTreeType>();
            foreach (DirectoryTreeType t in x)
            {
                if (t is DirectoryTreeTypeFile)
                {
                    DirectoryTreeTypeFile dttf = t as DirectoryTreeTypeFile;
                    if (dttf.GetFileType() == type)
                    {
                        return dttf;
                    }
                }
            }

            return DirectoryTreeType.Unknown;
        }
    }
}
