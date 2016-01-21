import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class PhotoCombine {

	private final static int margin = 85;
	private final static Color bg = Color.WHITE;

	public static void main(String[] args) {
		String name = args[0]; // name der dateien ohne endung
		String pathInput = args[1];
		String pathOutput = args[2];

		try {
			combineImages(name, pathInput, pathOutput);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static void combineImages(String name, String pathInput, String pathOutput) throws IOException {

		BufferedImage img1 = ImageIO.read(new File(pathInput + name + "_1.jpg"));
		BufferedImage img2 = ImageIO.read(new File(pathInput + name + "_2.jpg"));
		BufferedImage img3 = ImageIO.read(new File(pathInput + name + "_3.jpg"));
		BufferedImage img4 = ImageIO.read(new File(pathInput + name + "_4.jpg"));

		int height = img1.getHeight();
		int width = img1.getWidth();

		int combined_height = height * 2 + margin;
		int combined_width = width * 2 + margin;

		double scale = (double) height / combined_height;

		BufferedImage combined = new BufferedImage(width, height, img1.getType());
		Graphics2D g = combined.createGraphics();
		
		g.scale(scale, scale);
		g.setColor(bg);
		g.fillRect(0, 0, combined_width, combined_height);

		g.drawImage(img2, 0, 0, width, height, null); // oben links
		g.drawImage(img4, width + margin, 0, width, height, null); // oben rechts
		g.drawImage(img1, 0, height + margin, width, height, null); // unten links
		g.drawImage(img3, width + margin, height + margin, width, height, null); // unten rechts

		g.dispose();

		ImageIO.write(combined, "jpg", new File(pathOutput + name + ".jpg"));
	}
}
