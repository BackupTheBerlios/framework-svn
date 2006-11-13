using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
//using Microsoft.DirectX;
//using Microsoft.DirectX.Direct3D;


namespace NetDraw
{
	/// <summary>
	/// Summary description for BufferPictureBox.
	/// </summary>
	public class BufferPictureBox : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		//Bpublic delegate void DivBySevenHandler(object o, DivBySevenEventArgs e);
		public event PaintEventHandler PrePaint;
		public bool AlwaysClear;
		public new int Width
		{
			get 
			{
				return base.Width;
			}
			set
			{
				base.Width = value;
			}
		}
		public new int Height
		{
			get 
			{
				return base.Height;
			}
			set
			{
				base.Height = value;
			}
		}

		public BufferPictureBox()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			Application.ApplicationExit += 
				new EventHandler(MemoryCleanup);

			SetStyle(ControlStyles.AllPaintingInWmPaint, true);

			BackBuffer = new Bitmap(this.Width, this.Height);
			BufferGraphics = Graphics.FromImage(BackBuffer);
		}/*
			InicializarGraficos();
		}

		public void InicializarGraficos()
		{
			PresentParameters presentParams = new PresentParameters();
			presentParams.SwapEffect = SwapEffect.Discard;
			Format current = Microsoft.DirectX.Direct3D.Manager.Adapters[0].CurrentDisplayMode.Format;
			presentParams.BackBufferFormat = current;
			presentParams.BackBufferWidth = ScreenWidth;
			presentParams.BackBufferHeight = ScreenHeight;
			presentParams.Windowed = true;
			device = new Microsoft.DirectX.Direct3D.Device(0, Microsoft.DirectX.Direct3D.DeviceType.Hardware, this, CreateFlags.SoftwareVertexProcessing, presentParams);
			device.RenderState.AlphaBlendEnable = true;
			device.RenderState.SourceBlend = Blend.SourceAlpha;
			device.RenderState.DestinationBlend = Blend.InvSourceAlpha;
		}
		*/
		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}
		const Bitmap NO_BACK_BUFFER = null;
		const Graphics NO_BUFFER_GRAPHICS = null;

		Bitmap BackBuffer;
		Graphics BufferGraphics;


		private void MemoryCleanup(object sender, EventArgs e)
		{
			// clean up the memory
			if (BackBuffer != NO_BACK_BUFFER)
				BackBuffer.Dispose();

			if (BufferGraphics != NO_BUFFER_GRAPHICS)
				BufferGraphics.Dispose();
		}
		public void DoDraw()
		{
			PaintEventArgs pe = new PaintEventArgs(this.CreateGraphics(), this.ClientRectangle);
			OnPaint(pe);
		}
		public void DoClear()
		{
			//BufferGraphics.FillRectangle(Brushes.White, pe.ClipRectangle);
			BufferGraphics.FillRectangle(Brushes.White, BufferGraphics.ClipBounds);
		}
		protected override void OnPaint(PaintEventArgs pe)
		{
			if (PrePaint != null)
			{
				if (AlwaysClear)
					DoClear();
				//BufferGraphics.Transform = pe.Graphics.Transform;
				PaintEventArgs pq = new PaintEventArgs(BufferGraphics, pe.ClipRectangle);
				PrePaint(this, pq);
			}

			// now we draw the image into the screen
			pe.Graphics.DrawImageUnscaled(BackBuffer, 0, 0);
		}
		protected override void OnResize(EventArgs e)
		{
			if (BackBuffer != NO_BACK_BUFFER)
				BackBuffer.Dispose();

			BackBuffer = new Bitmap(this.Width, this.Height);
			BufferGraphics = Graphics.FromImage(BackBuffer);

			this.Refresh();
		}
		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// BufferPictureBox
			// 
			this.Name = "BufferPictureBox";
			
		}
		#endregion
	}
}
