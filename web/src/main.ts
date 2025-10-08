// NOTE: You'll need to generate your own base64 string from a sample processed image.
const staticBase64Image = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAA...';

class FrameViewer {
    private imageElement: HTMLImageElement;
    private statsElement: HTMLDivElement;

    constructor() {
        this.imageElement = document.getElementById('processed-frame') as HTMLImageElement;
        this.statsElement = document.getElementById('stats') as HTMLDivElement;
    }

    public displayFrame(): void {
        if (this.imageElement) {
            this.imageElement.src = staticBase64Image;
        }
    }

    public displayStats(): void {
        const stats = {
            resolution: '640x480',
            fps: 15 // Mock FPS
        };
        if (this.statsElement) {
            this.statsElement.innerText = `Resolution: ${stats.resolution} | FPS: ${stats.fps}`;
        }
    }
}

// Run the viewer
const viewer = new FrameViewer();
viewer.displayFrame();
viewer.displayStats();