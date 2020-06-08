// [dependencies]
// svg = "0.8.0"

fn sierpinski_pentagon(mut document : svg::Document, mut x : f64, mut y : f64,
        mut side : f64, order: usize) -> svg::Document {
    use svg::node::element::Path;
    use svg::node::element::path::Data;
    use std::f64::consts::PI;

    let degrees72 = 0.4 * PI;
    let mut angle = 3.0 * degrees72;
    let scale_factor = 1.0/(2.0 + degrees72.cos() * 2.0);

    if order == 1 {
        let mut data = Data::new();
        data = data.move_to((x, y));
        for _ in 0..5 {
            x += angle.cos() * side;
            y -= angle.sin() * side;
            angle += degrees72;
            data = data.line_to((x, y));
        }
        data = data.close();
        let path = Path::new()
            .set("fill", "blue")
            .set("stroke", "black")
            .set("stroke-width", "1")
            .set("d", data);
        document = document.add(path);
    } else {
        side *= scale_factor;
        let distance = side + side * degrees72.cos() * 2.0;
        for _ in 0..5 {
            x += angle.cos() * distance;
            y -= angle.sin() * distance;
            angle += degrees72;
            document = sierpinski_pentagon(document, x, y, side, order - 1);
        }
    }
    document
}

fn write_sierpinski_pentagon(file : &str, size : usize,
        order : usize) -> std::io::Result<()> {
    use svg::node::element::Rectangle;
    use std::f64::consts::PI;

    let margin = 5.0;
    let radius = (size as f64)/2.0 - 2.0 * margin;
    let side = radius * (0.2 * PI).sin() * 2.0;
    let x = (size as f64)/2.0;
    let y = 3.0 * margin;

    let rect = Rectangle::new()
        .set("width", "100%")
        .set("height", "100%")
        .set("fill", "white");

    let mut document = svg::Document::new()
        .set("width", size)
        .set("height", size)
        .add(rect);

    document = sierpinski_pentagon(document, x, y, side, order);
    svg::save(file, &document)
}

fn main() {
    write_sierpinski_pentagon("sierpinski_pentagon.svg", 600, 5).unwrap();
}