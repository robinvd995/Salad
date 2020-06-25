using GlmNet;
using SharpGL;
using SharpGL.RenderContextProviders;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace FruitSalad
{
    /// <summary>
    /// Interaction logic for SharpGLWindow.xaml
    /// </summary>
    public partial class SharpGLWindow : Window
    {
        private uint vaoID;

        static float minX = -0.5f, minY = -0.5f, minZ = -0.5f, minU = 0.0f, minV = 0.0f;
        static float maxX = 0.5f, maxY = 0.5f, maxZ = 0.5f, maxU = 1.0f, maxV = 1.0f;

        float[] vertexData = {
            minX, minY, minZ, /*minU, minV, 0.0f, 0.0f, 0.0f,*/
            maxX, minY, minZ, /*maxU, minV, 0.0f, 0.0f, 0.0f,*/
            minX, minY, maxZ, /*minU, maxV, 0.0f, 0.0f, 0.0f,*/
            maxX, minY, maxZ,/* maxU, maxV, 0.0f, 0.0f, 0.0f,*/

            minX, maxY, minZ, /*minU, maxV, 0.0f, 0.0f, 0.0f,*/
            maxX, maxY, minZ, /*maxU, maxV, 0.0f, 0.0f, 0.0f,*/
            minX, maxY, maxZ, /*minU, minV, 0.0f, 0.0f, 0.0f,*/
            maxX, maxY, maxZ, /*maxU, minV, 0.0f, 0.0f, 0.0f*/
        };

        float[] indexData = {
            0, 1, 2, 1, 3, 2, // Bottom Face
		    4, 5, 6, 5, 7, 6, // Top Face
		    0, 1, 5, 0, 5, 4, // Front Face
		    2, 3, 7, 2, 7, 6, // Back Face
		    0, 2, 6, 0, 6, 4, // Left Face
		    1, 3, 7, 1, 7, 5  // Right Face
	    };

        public SharpGLWindow()
        {
            InitializeComponent();
        }

        private void OpenGLControl_OpenGLInitialized(object sender, SharpGL.SceneGraph.OpenGLEventArgs args)
        {
            OpenGL gl = args.OpenGL;

            Console.WriteLine(args.OpenGL.Version);
            gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);

            uint[] arrays = new uint[1];
            gl.GenVertexArrays(arrays.Length, arrays);
            gl.BindVertexArray(arrays[0]);

            uint[] indexBuffers = new uint[1];
            gl.GenBuffers(indexBuffers.Length, indexBuffers);
            gl.BindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, indexBuffers[0]);
            gl.BufferData(OpenGL.GL_ELEMENT_ARRAY_BUFFER, indexData, OpenGL.GL_STATIC_DRAW);

            uint[] vertexBuffers = new uint[1];
            gl.GenBuffers(vertexBuffers.Length, vertexBuffers);
            gl.BindBuffer(OpenGL.GL_ARRAY_BUFFER, vertexBuffers[0]);
            gl.BufferData(OpenGL.GL_ARRAY_BUFFER, vertexData, OpenGL.GL_STATIC_DRAW);
            gl.VertexAttribPointer(0, 3, OpenGL.GL_FLOAT, false, 0, new IntPtr(0));

            gl.BindVertexArray(0);
            vaoID = arrays[0];
        }

        private void OpenGLControl_OpenGLDraw(object sender, SharpGL.SceneGraph.OpenGLEventArgs args)
        {
            OpenGL gl = args.OpenGL;
            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);

            gl.BindVertexArray(vaoID);
            gl.EnableVertexAttribArray(0);
            gl.DrawElements(OpenGL.GL_TRIANGLES, indexData.Length / 3, null);
            gl.DisableVertexAttribArray(0);
            gl.BindVertexArray(0);
        }

        private void OpenGLControl_Resized(object sender, SharpGL.SceneGraph.OpenGLEventArgs args)
        {

        }
    }
}
