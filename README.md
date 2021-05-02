# morse-decoder-encoder
**Arduino morse code decoder/encoder**

  This program encodes text input to the serial monitor to morse code via a flashing LED. 
  When the side button is pressed, the program switches to decoder mode, and by pressing
  a morse code pattern to a button (connected to a LED which will flash when the button 
  is pressed), the code will be decoded to text printed to the serial monitor.
  English language alphabet letters and number digits are acceptable.
  
  ![morse code tree](https://user-images.githubusercontent.com/35773069/116804085-f24d3e00-aad0-11eb-8663-851bd292c644.png)

  The morse code is represented as a binary tree, stored in a 0-based array, in which the left child of index i is found at index 2i + 1 and the right child of index i is found at 2i + 2.
  
  
  See more at: https://www.tinkercad.com/things/gPjcbHwequr-binary-tree-morse-code-encoder-decoder/editel
