using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class Tree<T>
    {
        public TreeNode<T> RootNode;

        public Tree() { }

        public Tree(T rootData)
        {
            RootNode = new TreeNode<T>(rootData);
        }

        public Tree(ITreePopulator<T> populator)
        {
            RootNode = populator.Populate();
        }

        public void AddSubBranch(TreeNode<T> node)
        {
            RootNode.Children.Add(node);
        }

        public void AddSubBranch(ITreePopulator<T> populator)
        {
            RootNode.Children.Add(populator.Populate());
        }
    }

    public class TreeNode<T>
    {
        public List<TreeNode<T>> Children { get; private set; }
        public T NodeData { get; private set; }

        public TreeNode(T data)
        {
            Children = new List<TreeNode<T>>();
            NodeData = data;
        }

        public TreeNode<T> AddChild(T data)
        {
            TreeNode<T> child = new TreeNode<T>(data);
            Children.Add(child);
            return child;
        }
    }

    public interface ITreePopulator<T>
    {
        TreeNode<T> Populate();
    }
}
