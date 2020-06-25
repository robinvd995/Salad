using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

using FruitSalad.Util;

namespace FruitSalad.XML
{
    class DataNode : IEnumerable<DataNode>, IFilterable<DataNode>
    {
        private readonly IDictionary<string, string> _attributes;

        public DataNode(int nodeType)
        {
            NodeType = nodeType;
            _attributes = new Dictionary<string,string>();
            Children = new List<DataNode>();
        }

        public int NodeType { get; private set; }
        public IList<DataNode> Children { get; private set; }
        public DataNode Parent { get; set; }
        
        public void AddAttribute(string attribName, string attribValue)
        {
            _attributes.Add(attribName, attribValue);
            OnAttributeAdded(attribName, attribValue);
        }

        public string GetAttribute(string attribName)
        {
            if (!_attributes.ContainsKey(attribName))
            {
                Logger.LogMessage(string.Format("ERROR: Trying to get attribute with name {0} but does not exist!", attribName));
                return string.Empty;
            }
            else
            {
                return _attributes[attribName];
            }
        }

        public virtual void OnNodeIntialized() { }
        public virtual void LoadData(string data) { }
        public virtual void OnNodeFinalized() { }

        protected virtual void OnAttributeAdded(string attribName, string attribValue) { }

        public IEnumerator<DataNode> GetEnumerator()
        {
            return new NodeEnumerator(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public IEnumerator<DataNode> GetFilteredEnumerator(Predicate<DataNode> predicate)
        {
            return new NodeFilterator(this, predicate);
        }

        public DataNode GetSingle(int nodeType)
        {
            IEnumerator<DataNode> enumerator = GetEnumerator();
            while (enumerator.MoveNext())
            {
                DataNode current = enumerator.Current;
                if(current.NodeType == nodeType)
                {
                    return current;
                }
            }

            return null;
        }
    }

    class NodeEnumerator : IEnumerator<DataNode>
    {
        protected Queue<DataNode> _dataNodes;

        public NodeEnumerator()
        {
            _dataNodes = new Queue<DataNode>();
        }

        public NodeEnumerator(DataNode rootNode)
        {
            _dataNodes = new Queue<DataNode>();
            AddNodeToQueue(rootNode);
        }

        protected virtual void AddNodeToQueue(DataNode node)
        {
            _dataNodes.Enqueue(node);
            node.Children.ToList().ForEach(c => AddNodeToQueue(c));
        }

        public DataNode Current { get; private set; }

        object IEnumerator.Current { get { return Current; } }

        public void Dispose()
        {
            _dataNodes.Clear();
        }

        public bool MoveNext()
        {
            bool flag = _dataNodes.Count > 0;
            if (flag)
            {
                Current = _dataNodes.Dequeue();
            }
            return flag;
        }

        public void Reset()
        {
            throw new NotImplementedException();
        }
    }

    class NodeFilterator : NodeEnumerator
    {
        private readonly Predicate<DataNode> _predicate;

        public NodeFilterator(DataNode rootNode, Predicate<DataNode> predicate) : base()
        {
            _predicate = predicate;
            AddNodeToQueue(rootNode);
        }

        protected override void AddNodeToQueue(DataNode node)
        {
            if (_predicate(node)) {
                _dataNodes.Enqueue(node);
            }
            node.Children.ToList().ForEach(c => AddNodeToQueue(c));
        }
    }
}
