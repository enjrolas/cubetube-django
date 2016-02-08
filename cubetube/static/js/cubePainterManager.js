var kBoardWidth = 8;
var kBoardHeight= 8;
var kPickerWidth = 3;
var kPickerHeight= 5;
var kNumPieces = 64;
var kPieceWidth = 50;
var kPieceHeight= 50;
var kPixelWidth = 1 + (kBoardWidth * kPieceWidth);
var kPixelHeight= 1 + (kBoardHeight * kPieceHeight);
var kPickerWidth = 1 + (kPickerWidth * kPieceWidth);
var kPickerHeight= 1 + (kPickerHeight * kPieceHeight);

var gCanvasElement;
var gDrawingContext;
var gPickerElement;
var gPickerContext;

var gLayer;
var gPieces;
var gColors;
var gNumPieces;
var gNumColors;
var gSelectedPieceIndex;
var gPickedColorIndex;
//var gSelectedPieceHasMoved;
var gLayerCountElem;
var gGameInProgress;

function Cell(row, column, fillColor, isFilled) {
    this.row = row;
    this.column = column;
    this.fillColor = fillColor;
    this.isFilled = isFilled;
}

function getCursorPosition(e) {
    /* returns Cell with .row and .column properties */
    var x, newX;
    var y, newY;
    
    if (e.pageX != undefined && e.pageY != undefined) {
		x = e.pageX;
		y = e.pageY;
    }
    else {
		x = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
		y = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
    }
    
    x -= gCanvasElement.offsetLeft;
    y -= gCanvasElement.offsetTop;
    x = Math.min(x, kBoardWidth * kPieceWidth);
    y = Math.min(y, kBoardHeight * kPieceHeight);
    
    newX = Math.floor(y/kPieceHeight);
    newY = Math.floor(x/kPieceWidth);
    return getCell(newX, newY);
}

function getCell(row, column) {
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++) {
		if ((gPieces[idx].row == row) && 
		    (gPieces[idx].column == column)) {
		    return gPieces[idx];
		}
    }
}

function increaseLayer() {
	if(gLayer < 7)
		gLayer++;
	else
		gLayer = 7;
	drawCube();
}

function decreaseLayer() {
	if(gLayer > 0)
		gLayer--;
	else
		gLayer = 0;
	drawCube();
}

function cubeOnClick(e) {
    var cell = getCursorPosition(e);
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++) {
		if ((gPieces[idx].row == cell.row) && 
		    (gPieces[idx].column == cell.column)) {
		    clickOnPiece(idx);
		    return;
		}
    }
}

function pickerOnClick(e) {
    var cell = getCursorPosition(e);
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++) {
		if ((gPieces[idx].row == cell.row) && 
		    (gPieces[idx].column == cell.column)) {
		    clickOnPiece(idx);
		    return;
		}
    }
}

function clickOnPiece(pieceIndex) {
    //if (gSelectedPieceIndex == pieceIndex) { return; }
    gSelectedPieceIndex = ((gLayer*kNumPieces) + (gPieces[pieceIndex].column*(kNumPieces/8)) + (kBoardHeight - gPieces[pieceIndex].row)) - 1;
    console.log('Piece index: ' + gSelectedPieceIndex);
    
    gSelectedPieceHasMoved = false;
    gPieces[pieceIndex].isFilled = !gPieces[pieceIndex].isFilled; 
    gPieces[pieceIndex].fillColor = gPieces[pieceIndex].isFilled ? $('#colors').find(":selected").val() : '#000000';
    
    setVoxel(gSelectedPieceIndex, gPieces[pieceIndex].fillColor);
    drawCube();
}

function updateLayerCountElem() {
	if(gLayer === 7)
		gLayerCountElem.html(" Front ");
	else if(gLayer === 0)
		gLayerCountElem.html(" Back ");
	else
		gLayerCountElem.html("Layer: " + (gLayer+1));
}

function clearPieces(startIdx, endIdx) {
	for(var pieceIndex=startIdx; pieceIndex<endIdx; pieceIndex++) {
	    gPieces[pieceIndex].isFilled = false;
	    gPieces[pieceIndex].fillColor = "#000000";
	}
	drawCube();
}

function drawCube() {
    if (gGameInProgress && isTheGameOver())
    	endGame();

    gDrawingContext.clearRect(0, 0, kPixelWidth, kPixelHeight);
    gDrawingContext.beginPath();
    
    /* vertical lines */
    for (var x = 0; x <= kPixelWidth; x += kPieceWidth) {
		gDrawingContext.moveTo(0.5 + x, 0);
		gDrawingContext.lineTo(0.5 + x, kPixelHeight);
    }
    
    /* horizontal lines */
    for (var y = 0; y <= kPixelHeight; y += kPieceHeight) {
		gDrawingContext.moveTo(0, 0.5 + y);
		gDrawingContext.lineTo(kPixelWidth, 0.5 +  y);
    }
    
    /* draw it! */
    gDrawingContext.strokeStyle = "#ccc";
    gDrawingContext.stroke();
    
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++)
    	drawPiece(gPieces[idx], gPieces[idx].isFilled);

    updateLayerCountElem();
    
    saveGameState();
}

function drawPiece(p, isFilled) {
    var column = p.column;
    var row = p.row;
    var x = (column * kPieceWidth) + (kPieceWidth/2);
    var y = (row * kPieceHeight) + (kPieceHeight/2);
    var radius = (kPieceWidth/2) - (kPieceWidth/10);
    
    gDrawingContext.beginPath();
    gDrawingContext.arc(x, y, radius, 0, Math.PI*2, false);
    gDrawingContext.closePath();
    gDrawingContext.strokeStyle = isFilled ? p.fillColor : "#000";
    gDrawingContext.stroke();
    
    if (isFilled) {
		gDrawingContext.fillStyle = p.fillColor;
		gDrawingContext.fill();
    }
}

function drawPicker() {
    gPickerContext.clearRect(0, 0, kPickerWidth, kPickerHeight);
    gPickerContext.beginPath();
    
    /* vertical lines */
    for (var x = 0; x <= kPixelWidth; x += kPickerWidth) {
		gPickerContext.moveTo(0.5 + x, 0);
		gPickerContext.lineTo(0.5 + x, kPickerHeight);
    }
    
    /* horizontal lines */
    for (var y = 0; y <= kPickerHeight; y += kPieceHeight) {
		gPickerContext.moveTo(0, 0.5 + y);
		gPickerContext.lineTo(kPickerWidth, 0.5 +  y);
    }
    
    /* draw it! */
    gPickerContext.strokeStyle = "#ccc";
    gPickerContext.stroke();
    
    for (var idx=0; idx < gColors.length; idx++)
    	drawColor(gColors[idx], gColors[idx].isFilled);
}

function drawColor(p, isFilled) {
    var column = p.column;
    var row = p.row;
    var x = (column * kPieceWidth) + (kPieceWidth/2);
    var y = (row * kPieceHeight) + (kPieceHeight/2);
    var radius = (kPieceWidth/2) - (kPieceWidth/10);
    
    gPickerContext.beginPath();
    gPickerContext.arc(x, y, radius, 0, Math.PI*2, false);
    gPickerContext.closePath();
    gPickerContext.strokeStyle = p.fillColor;
    gPickerContext.stroke();
	gPickerContext.fillStyle = p.fillColor;
	gPickerContext.fill();
}

function supportsLocalStorage() {
    return ('localStorage' in window) && window['localStorage'] !== null;
}

function saveGameState() {
    if (!supportsLocalStorage()) { return false; }
    
    localStorage["halma.game.in.progress"] = gGameInProgress;
    
    for (var i = 0; i < kNumPieces; i++) {
		localStorage["halma.piece." + i + ".row"] = gPieces[i].row;
		localStorage["halma.piece." + i + ".column"] = gPieces[i].column;
    }
    
    localStorage["halma.selectedpiece"] = gSelectedPieceIndex;
    localStorage["halma.selectedpiecehasmoved"] = gSelectedPieceHasMoved;
    //localStorage["halma.movecount"] = gMoveCount;
    
    return true;
}

function resumeGame() {
    if (!supportsLocalStorage()) { return false; }
    
    gGameInProgress = (localStorage["halma.game.in.progress"] == "true");
    if (!gGameInProgress) { return false; }
    
    gPieces = new Array(kNumPieces);
    
    for (var i = 0; i < kNumPieces; i++) {
		var row = parseInt(localStorage["halma.piece." + i + ".row"]);
		var column = parseInt(localStorage["halma.piece." + i + ".column"]);
		gPieces[i] = new Cell(row, column);
    }
    
    gNumPieces = kNumPieces;
    gSelectedPieceIndex = parseInt(localStorage["halma.selectedpiece"]);
    gSelectedPieceHasMoved = localStorage["halma.selectedpiecehasmoved"] == "true";
    //gMoveCount = parseInt(localStorage["halma.movecount"]);
    
    drawCube();
    return true;
}

function newGame() {
	gPieces = new Array();
	for (var z=0; z<8; z++)
		for (var r=0; r<8; r++)
			for (var c=0; c<8; c++)
				gPieces.push(new Cell(r, c, "#00000", false));
    
	gColors = new Array(new Cell(0, 0, "#929591", true),
						new Cell(1, 0, "#ffff14", true),
						new Cell(2, 0, "#fdf5e6", true),
						new Cell(3, 0, "#c20078", true),
						new Cell(4, 0, "#f97306", true),
						new Cell(0, 1, "#029386", true),
						new Cell(1, 1, "#02ffff", true),
						new Cell(2, 1, "#e50000", true),
						new Cell(3, 1, "#653700", true),
						new Cell(4, 1, "#ff81c0", true),
						new Cell(0, 2, "#ffc0cb", true),
						new Cell(1, 2, "#0343df", true),
						new Cell(2, 2, "#15b01a", true),
						new Cell(3, 2, "#7e1e9c", true),
						new Cell(4, 2, "#ffffff", true));
		
    gNumPieces = gPieces.length;
    gSelectedPieceIndex = -1;
    gSelectedPieceHasMoved = false;
    gLayer = 7;
    gGameInProgress = true;
    
    drawCube();
    //drawPicker();
}

function isTheGameOver() {
    for (var i = 0; i < gNumPieces; i++) {
		if (gPieces[i].row > 2)
		    return false;

		if (gPieces[i].column < (kBoardWidth - 3))
		    return false;
    }
    
    return true;
}

function endGame() {
    gSelectedPieceIndex = -1;
    gGameInProgress = false;
}

function initGame(canvasElement, pickerElement) {
    gCanvasElement = canvasElement;
    gPickerElement = pickerElement;
    gCanvasElement.width = kPixelWidth;
    gCanvasElement.height = kPixelHeight;
    gPickerElement.width = kPickerWidth;
    gPickerElement.height = kPickerHeight;
    gCanvasElement.addEventListener("click", cubeOnClick, false);
    gPickerElement.addEventListener("click", pickerOnClick, false);
    gDrawingContext = gCanvasElement.getContext("2d");
    gPickerContext = gPickerElement.getContext("2d");
    gLayerCountElem = $("span#currentLayer");
    
    //if (!resumeGame())
    	newGame();
}