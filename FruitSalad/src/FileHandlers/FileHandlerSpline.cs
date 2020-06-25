using FruitSalad.Director;
using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace FruitSalad.FileHandlers
{
    public class DirectoryTreeTypeSpline : DirectoryTreeTypeFile
    {
        public DirectoryTreeTypeSpline(int id, string name, FileType type) :
            base(id, name, type)
        {

        }

        protected override void AddActions()
        {
            base.AddActions();
        }

        public override void HandleAction(string eventId, DirectorItemData data)
        {
            base.HandleAction(eventId, data);
        }

        protected override FileContainer OpenFile(FSFile file)
        {
            FileContainer container = new FileContainer();
            container.Source = new SourceContainerSpline(file.AbosultePath);
            return container;
        }
    }

    public class SourceContainerSpline : SourceContainer
    {
        private SourceContainerSplineVM viewModel;

        public float TranslateX { get; set; }
        public float TranslateY { get; set; }
        public float ScaleX { get; set; }
        public float ScaleY { get; set; }

        public SourceContainerSpline(string filePath) :
            base(filePath)
        {
            viewModel = new SourceContainerSplineVM();

            // Loading spline points
            Spline2f spline = new Spline2f(true);
            spline.AddPoint(28, 60);
            spline.AddPoint(40, 80);
            spline.AddPoint(70, 83);
            spline.AddPoint(100, 83);
            spline.AddPoint(130, 83);
            spline.AddPoint(160, 80);
            spline.AddPoint(172, 60);
            spline.AddPoint(160, 40);
            spline.AddPoint(130, 37);
            spline.AddPoint(100, 37);
            spline.AddPoint(70, 37);
            spline.AddPoint(40, 40);
            
            TranslateX = 100.0f;
            TranslateY = 100.0f;
            ScaleX = 4.0f;
            ScaleY = 4.0f;

            bool first = true;
            SplinePoint2f prevPoint = null;
            for(float f = 0; f <= 12.0f; f += 0.1f)
            {
                SplinePoint2f splinePoint = spline.GetSplinePoint(f);
                if(!first)
                {
                    Tuple<float, float> prevCoord = TransformCoordinate(prevPoint.X, prevPoint.Y);
                    Tuple<float, float> curCoord = TransformCoordinate(splinePoint.X, splinePoint.Y);

                    SplineDataVM data = new SplineDataVM
                    {
                        Geometry = new LineGeometry() { 
                            StartPoint = new System.Windows.Point() { X = prevCoord.Item1, Y = prevCoord.Item2 }, 
                            EndPoint = new System.Windows.Point() { X = curCoord.Item1, Y = curCoord.Item2 } 
                        },
                        Fill = Brushes.Blue,
                        Stroke = Brushes.Blue,
                        StrokeThickness = 2
                    };
                    viewModel.SplineData.Add(data);
                }
                prevPoint = splinePoint;
                first = false;
            }

            foreach(SplinePoint2f splinePoint in spline.SplinePoints)
            {
                Tuple<float, float> curCoord = TransformCoordinate(splinePoint.X, splinePoint.Y);
                SplineDataVM data = new SplineDataVM
                {
                    Geometry = new RectangleGeometry()
                    {
                        Rect = new System.Windows.Rect(curCoord.Item1 - 2.0D, curCoord.Item2 - 2.0D, 4.0D, 4.0D)
                    },
                    Fill = Brushes.Red,
                    Stroke = Brushes.Red,
                    StrokeThickness = 2
                };
                viewModel.SplineData.Add(data);
            }
        }

        private Tuple<float,float> TransformCoordinate(float x, float y)
        {
            float newx = (x * ScaleX) + TranslateX;
            float newy = (y * ScaleY) + TranslateY;
            return new Tuple<float, float>(newx, newy);
        }

        public override SourceContainerVM GetViewModel()
        {
            return viewModel;
        }

        public override void SaveSource()
        {
            //throw new NotImplementedException();
        }
    }

    public class SplineDataVM
    {
        public Geometry Geometry { get; set; }
        public Brush Fill { get; set; }
        public Brush Stroke { get; set; }
        public double StrokeThickness { get; set; }
    }

    public class SourceContainerSplineVM : SourceContainerVM
    {
        public SourceContainerSplineVM()
        {
            SplineData = new List<SplineDataVM>();
        }

        public override FileType GetFileType()
        {
            return FileType.Spline;
        }

        public List<SplineDataVM> SplineData { get; set; }
    }
}
