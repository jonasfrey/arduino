
import { decode } from "https://deno.land/x/pngs/mod.ts";

const file = await Deno.readFile("./nuoviso_logo.png");
let o_img = (decode(file));


let s_code = ``;


class O_rgb{
    constructor(
        n_r, 
        n_g, 
        n_b
    ){
        this.n_r = n_r, 
        this.n_g = n_g, 
        this.n_b = n_b
    }
}
class O_rgba{
    constructor(
        n_r, 
        n_g, 
        n_b, 
        n_a
    ){
        this.n_r = n_r, 
        this.n_g = n_g, 
        this.n_b = n_b, 
        this.n_a = n_a
    }
}
let f_o_rgb_from_o_rgba= function(o_rgb_background, o_rgba)
{
    var n_alpha = o_rgba.n_a;

    return new O_rgb(
        (1 - n_alpha) * o_rgb_background.n_r + n_alpha * o_rgba.n_r,
        (1 - n_alpha) * o_rgb_background.n_g + n_alpha * o_rgba.n_g,
        (1 - n_alpha) * o_rgb_background.n_b + n_alpha * o_rgba.n_b
    );
}
console.log(o_img)
// Deno.exit()
let a_n_u16 = [];
let n_idx = 0;
for(let n_x = 0; n_x < o_img.width; n_x +=1){
    for(let n_y = 0; n_y < o_img.height; n_y +=1){

        let o_rgb__background = new O_rgb(0,0,0);
        let o_rgba = new O_rgba(
            o_img.image[n_idx+0]/255, 
            o_img.image[n_idx+1]/255, 
            o_img.image[n_idx+2]/255, 
            o_img.image[n_idx+3]/255
        );
        let o_rgb_nor = f_o_rgb_from_o_rgba(
            o_rgb__background, 
            o_rgba
        );
        let o_rgb = new O_rgb(
            parseInt(o_rgb_nor.n_r*255),
            parseInt(o_rgb_nor.n_g*255),
            parseInt(o_rgb_nor.n_b*255),
        ); 
        let n_u24 = 0; 
        n_u24 = n_u24 | (o_rgb.n_b<<(8*0));
        n_u24 = n_u24 | (o_rgb.n_g<<(8*1));
        n_u24 = n_u24 | (o_rgb.n_r<<(8*2));
        let n_nor = (n_u24 / ((1<<24)-1));
        let n_u16 = (n_u24 / ((1<<24)-1)) * ((1<<16)-1);
        if(n_nor < 0 || true){
            console.log()
            console.log('o_rgba');
            console.log(o_rgba);
            console.log('o_rgb');
            console.log(o_rgb);
            console.log('n_u24');
            console.log(n_u24);
            console.log('n_nor');
            console.log(n_nor);
            console.log('n_u16');
            console.log(n_u16);
        }
        a_n_u16.push(parseInt(n_u16));
        if(
            n_nor > 0
        ){
            s_code+=`o_tft.drawPixel(${n_x}, ${n_y}, ${parseInt(n_u16)});\r\n`
        }
        n_idx+=1;
    
    }

}
// s_code += `
// uint16_t a_n_u16 [ ${a_n_u16.length} ] = {${a_n_u16.join(',')}};
// int n_idx = 0;
// for(int n_x = 0; n_x < ${o_img.width}; n_x +=1){
//     for(int n_y = 0; n_y < ${o_img.height}; n_y +=1){
//         n_idx+=1;
//         o_tft.drawPixel(n_x, n_y, a_n_u16[n_idx]);
//     }
// }
// `
Deno.writeTextFile("./image.ino", s_code);

