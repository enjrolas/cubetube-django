var kBoardWidth = 8;
var kBoardHeight= 8;
var kPickerWidth = 3;
var kPickerHeight= 5;
var kNumPieces = 64;
var kNumColors = 15;
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
var gLayerCountElem;
var gGameInProgress;

function Cell(row, column, fillColor, isFilled) {
    this.row = row;
    this.column = column;
    this.fillColor = fillColor;
    this.isFilled = isFilled;
}

function getCursorPosition(e, canvasElement, cellArray) {
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
    
    x -= canvasElement.offsetLeft;
    y -= canvasElement.offsetTop;
    x = Math.min(x, kBoardWidth * kPieceWidth);
    y = Math.min(y, kBoardHeight * kPieceHeight);
    
    newX = Math.floor(y/kPieceHeight);
    newY = Math.floor(x/kPieceWidth);
    return getCell(newX, newY, cellArray);
}

function getCell(row, column, cellArray) {
	if(cellArray.length > kNumColors) {
	    var i = gLayer*kNumPieces;
	    for (var idx=i; idx < (i+kNumPieces); idx++) {
			if ((cellArray[idx].row == row) && 
			    (cellArray[idx].column == column)) {
			    return cellArray[idx];
			}
	    }
    }
    else {
	    for (var idx=0; idx < cellArray.length; idx++) {
			if ((cellArray[idx].row == row) && 
			    (cellArray[idx].column == column)) {
			    return cellArray[idx];
			}
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

function updateLayerCountElem() {
	if(gLayer === 7)
		gLayerCountElem.html(" Front ");
	else if(gLayer === 0)
		gLayerCountElem.html(" Back ");
	else
		gLayerCountElem.html(" Layer: " + (gLayer+1) + " ");
}

function cubeOnClick(e) {
    var cell = getCursorPosition(e, gCanvasElement, gPieces);
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++) {
		if ((gPieces[idx].row == cell.row) && 
		    (gPieces[idx].column == cell.column)) {
		    clickOnPiece(idx, gPieces);
		    return;
		}
    }
}

function pickerOnClick(e) {
    var cell = getCursorPosition(e, gPickerElement, gColors);
    for (var idx=0; idx < kNumColors; idx++) {
		if ((gColors[idx].row == cell.row) && 
		    (gColors[idx].column == cell.column)) {
		    clickOnPiece(idx, gColors);
		    return;
		}
    }
}

function clickOnPiece(pieceIndex, cellArray) {
	if(cellArray.length > kNumColors)
		gSelectedPieceIndex = ((gLayer*kNumPieces) + (cellArray[pieceIndex].column*(kNumPieces/8)) + (kBoardHeight - cellArray[pieceIndex].row)) - 1;
	else
		gSelectedPieceIndex = pieceIndex;
	//console.log('Piece index: ' + gSelectedPieceIndex);
    
    if(cellArray.length > kNumColors) {
    	cellArray[pieceIndex].isFilled = !cellArray[pieceIndex].isFilled; 
	    cellArray[pieceIndex].fillColor = cellArray[pieceIndex].isFilled ? $('#colors').find(":selected").val() : '#000000';
	    setVoxel(gSelectedPieceIndex, cellArray[pieceIndex].fillColor);
	    drawCube();
    }
    else {
    	$('#colors').val(cellArray[pieceIndex].fillColor)
	    gGameInProgress = true;
	    saveGameState();
    }
    
}

function clearPieces(startIdx, endIdx) {
	for(var pieceIndex=startIdx; pieceIndex<endIdx; pieceIndex++) {
	    gPieces[pieceIndex].isFilled = false;
	    gPieces[pieceIndex].fillColor = "#000000";
	}
	drawCube();
}

function drawCube() {
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
    
    gGameInProgress = true;
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
    gDrawingContext.strokeStyle = isFilled ? "#ccc" : "#000";
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
	gPickerContext.moveTo(0.5, 0);
	gPickerContext.lineTo(0.5, kPickerHeight);
	gPickerContext.moveTo(kPickerWidth, 0);
	gPickerContext.lineTo(kPickerWidth, kPickerHeight);
    
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
    gPickerContext.strokeStyle = "#ccc";
    gPickerContext.stroke();
	gPickerContext.fillStyle = p.fillColor;
	gPickerContext.fill();
}

function supportsLocalStorage() {
    return ('localStorage' in window) && window['localStorage'] !== null;
}

function saveGameState() {
    if (!supportsLocalStorage()) { return false; }
    
    localStorage["cubetube.game.in.progress"] = gGameInProgress;
    
    for (var i = 0; i < gNumPieces; i++) {
		localStorage["cubetube.piece." + i + ".row"] = gPieces[i].row;
		localStorage["cubetube.piece." + i + ".column"] = gPieces[i].column;
		localStorage["cubetube.piece." + i + ".fillColor"] = gPieces[i].fillColor;
		localStorage["cubetube.piece." + i + ".isFilled"] = gPieces[i].isFilled;
    }
    
    localStorage["cubetube.currentlayer"] = gLayer;
    localStorage["cubetube.selectedpiece"] = gSelectedPieceIndex;
    localStorage["cubetube.selectedcolor"] = $("#colors").val();
    
    return true;
}

function resumeGame() {
    if (!supportsLocalStorage()) { return false; }
    
    gGameInProgress = (localStorage["cubetube.game.in.progress"] == "true");
    if (!gGameInProgress) { return false; }
    
    gNumPieces = kNumPieces*kBoardWidth;
    gPieces = new Array();
    
    for (var i = 0; i < gNumPieces; i++) {
		var row = parseInt(localStorage["cubetube.piece." + i + ".row"]);
		var column = parseInt(localStorage["cubetube.piece." + i + ".column"]);
		var fillColor = localStorage["cubetube.piece." + i + ".fillColor"];
		var isFilled = localStorage["cubetube.piece." + i + ".isFilled"] == 'true';
		gPieces.push(new Cell(row, column, fillColor, isFilled));
		//gPieces[i] = new Cell(row, column, fillColor, isFilled);
    }
	
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
    
    gLayer = parseInt(localStorage["cubetube.currentlayer"]);
    gSelectedPieceIndex = parseInt(localStorage["cubetube.selectedpiece"]);
    $("#colors").val(localStorage["cubetube.selectedcolor"]);
    
    drawCube();
    drawPicker();
    return true;
}

function newGame() {
	gPieces = new Array();
	for (var z=0; z<kBoardWidth; z++)
		for (var r=0; r<kBoardHeight; r++)
			for (var c=0; c<kBoardWidth; c++)
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
    gLayer = 7;
    gGameInProgress = false;
    
    drawCube();
    drawPicker();
}

/*function isTheGameOver() {
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
}*/

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
    
    if (!resumeGame())
    	newGame();
}