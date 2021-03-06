var kBoardWidth = 8;
var kBoardHeight= 8;
var kPickerWidth = 3;
var kPickerHeight= 8;
var kNumPieces = 64;
var kNumColors = 24;
var kPieceWidth = 50;
var kPieceHeight= 50;
var kPixelWidth = 1 + (kBoardWidth * kPieceWidth);
var kPixelHeight= 1 + (kBoardHeight * kPieceHeight);
var kPickerWidth = 1 + (kPickerWidth * kPieceWidth);
var kPickerHeight= 1 + (kPickerHeight * kPieceHeight);

var gVoxelImage;
var gCanvasElement;
var gDrawingContext;
var gPickerElement;
var gPickerContext;
var gSelectedColor;

var gLayer;
var gPieces;
var gColors;
var gNumPieces;
var gNumColors;
var gSelectedPieceIndex;
var gSelectedPieceArrayIndex;
var gPickedColorIndex;
var gLayerCountElem;
var gGameInProgress;

function Cell(row, column, zIndex, fillColor, isFilled) {
    this.row = row;
    this.column = column;
    this.zIndex = zIndex;
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
		gLayerCountElem.html("&nbsp;&nbsp;Front&nbsp;&nbsp;");
	else if(gLayer === 0)
		gLayerCountElem.html("&nbsp;&nbsp;Back&nbsp;&nbsp;");
	else
		gLayerCountElem.html("&nbsp;&nbsp;Layer: " + ((7 - gLayer) + 1) + "&nbsp;&nbsp;");
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
	if(cellArray.length > kNumColors) {
		gSelectedPieceIndex = ((gLayer*kNumPieces) + (cellArray[pieceIndex].column*(kNumPieces/kBoardWidth)) + (kBoardHeight - cellArray[pieceIndex].row)) - 1;
		gSelectedPieceArrayIndex = pieceIndex;
	}
	else {
		gSelectedPieceIndex = pieceIndex;
		gPickedColorIndex = pieceIndex;
	}
		
	//console.log('Piece index: ' + gSelectedPieceIndex);
    
    if(cellArray.length > kNumColors) {
    	cellArray[pieceIndex].zIndex = gLayer;
    	cellArray[pieceIndex].isFilled = !cellArray[pieceIndex].isFilled; 
	    cellArray[pieceIndex].fillColor = cellArray[pieceIndex].isFilled ? gSelectedColor : '#000000';
	    setVoxel(gSelectedPieceIndex, cellArray[pieceIndex].fillColor);
	    drawCube();
    }
    else {
    	gSelectedColor = cellArray[pieceIndex].fillColor;
    	drawPicker();
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
	
	/* voxel background image */
	drawBackground();
    
    /* vertical lines */
    for (var x = 0; x <= kPixelWidth; x += kPieceWidth) {
    	gDrawingContext.lineWidth = ((x == 0) || (x == kPixelWidth)) ? 4 : 1;
		gDrawingContext.moveTo(0.5 + x, 0);
		gDrawingContext.lineTo(0.5 + x, kPixelHeight);
    }
    
    /* horizontal lines */
    for (var y = 0; y <= kPixelHeight; y += kPieceHeight) {
    	gDrawingContext.lineWidth = ((y == 0) || (y == kPixelHeight)) ? 4 : 1;
		gDrawingContext.moveTo(0, 0.5 + y);
		gDrawingContext.lineTo(kPixelWidth, 0.5 +  y);
    }
    
    /* draw it! */
    gDrawingContext.strokeStyle = "#ccc";
    gDrawingContext.stroke();
	
    var i = gLayer*kNumPieces;
    for (var idx=i; idx < (i+kNumPieces); idx++)
    	drawPiece(gPieces[idx], gPieces[idx].isFilled, gSelectedPieceArrayIndex === idx);
	
    updateLayerCountElem();
    
    gGameInProgress = true;
    saveGameState();
}

/* voxel background image */
function drawBackground() {
	for (var x = 0; x <= kPixelWidth; x += kPieceWidth)
		for (var y = 0; y <= kPixelHeight; y += kPieceHeight)
			gDrawingContext.drawImage(gVoxelImage, x, y, kPieceWidth, kPieceHeight);
}

function drawPiece(p, isFilled, isSelected) {
    var column = p.column;
    var row = p.row;
    var x = (column * kPieceWidth) + (kPieceWidth/2);
    var y = (row * kPieceHeight) + (kPieceHeight/2);
    var radius = (kPieceWidth/2) - (kPieceWidth/10);
    
    gDrawingContext.beginPath();
    gDrawingContext.arc(x, y, radius, 0, Math.PI*2, false);
    gDrawingContext.closePath();
    gDrawingContext.strokeStyle = isSelected ? "#0343df" : isFilled ? p.fillColor : "#ccc";
    gDrawingContext.lineWidth = isSelected ? 4 : 2;
    gDrawingContext.stroke();
    
    if (isFilled) {
		gDrawingContext.globalAlpha = 0.8;	// Set transparency
		gDrawingContext.fillStyle = p.fillColor;
		gDrawingContext.fill();
    }
    else {
		gDrawingContext.globalAlpha = 0.2;	// Set transparency
		gDrawingContext.fillStyle = "#ffffff";
		gDrawingContext.fill();
    }
	// Turn transparency off
	gDrawingContext.globalAlpha = 1.0;
}

function drawPicker() {
    gPickerContext.clearRect(0, 0, kPickerWidth, kPickerHeight);
    gPickerContext.beginPath();
    
    /* vertical lines */
	gPickerContext.lineWidth = 2;
	gPickerContext.moveTo(0.5, 0);
	gPickerContext.lineTo(0.5, kPickerHeight);
	gPickerContext.moveTo(kPickerWidth, 0);
	gPickerContext.lineTo(kPickerWidth, kPickerHeight);
    
    /* horizontal lines */
    for (var y = 0; y <= kPickerHeight; y += kPieceHeight) {
    	gPickerContext.lineWidth = ((y == 0) || (y == kPickerHeight)) ? 2 : 4;
		gPickerContext.moveTo(0, 0.5 + y);
		gPickerContext.lineTo(kPickerWidth, 0.5 +  y);
    }
    
    /* draw it! */
    gPickerContext.strokeStyle = "#ccc";
    gPickerContext.stroke();
    
    for (var idx=0; idx < gColors.length; idx++)
    	drawColor(gColors[idx], gColors[idx].isFilled, gPickedColorIndex === idx);
    
    gGameInProgress = true;
    saveGameState();
}

function drawColor(p, isFilled, isSelected) {
    var column = p.column;
    var row = p.row;
    var x = (column * kPieceWidth) + (kPieceWidth/2);
    var y = (row * kPieceHeight) + (kPieceHeight/2);
    var radius = (kPieceWidth/2) - (kPieceWidth/10);
    
    gPickerContext.beginPath();
    gPickerContext.arc(x, y, radius, 0, Math.PI*2, false);
    gPickerContext.closePath();
    gPickerContext.strokeStyle = isSelected ? "#0343df" : "#ccc";
    gPickerContext.lineWidth = isSelected ? 4 : 2;
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
		localStorage["cubetube.piece." + i + ".zIndex"] = gPieces[i].zIndex;
		localStorage["cubetube.piece." + i + ".fillColor"] = gPieces[i].fillColor;
		localStorage["cubetube.piece." + i + ".isFilled"] = gPieces[i].isFilled;
    }
    
    localStorage["cubetube.currentlayer"] = gLayer;
    localStorage["cubetube.selectedpiece"] = gSelectedPieceIndex;
    localStorage["cubetube.selectedarray"] = gSelectedPieceArrayIndex;
    localStorage["cubetube.selectedcolorindex"] = gPickedColorIndex;
    localStorage["cubetube.selectedcolor"] = gSelectedColor;

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
		var zIndex = parseInt(localStorage["cubetube.piece." + i + ".zIndex"]);
		var fillColor = localStorage["cubetube.piece." + i + ".fillColor"];
		var isFilled = localStorage["cubetube.piece." + i + ".isFilled"] == 'true';
		gPieces.push(new Cell(row, column, zIndex, fillColor, isFilled));
    }
	
    gColors = new Array(new Cell(0, 0, -1, "#660000", true),
						new Cell(1, 0, -1, "#FF0000", true),
						new Cell(2, 0, -1, "#FF6666", true),
						new Cell(3, 0, -1, "#FF9999", true),
						new Cell(4, 0, -1, "#FFB266", true),
						new Cell(5, 0, -1, "#FF9933", true),
						new Cell(6, 0, -1, "#FF8000", true),
						new Cell(7, 0, -1, "#CC3300", true),
						new Cell(0, 1, -1, "#006600", true),
						new Cell(1, 1, -1, "#00FF00", true),
						new Cell(2, 1, -1, "#99FF99", true),
						new Cell(3, 1, -1, "#029386", true),
						new Cell(4, 1, -1, "#FFFF99", true),
						new Cell(5, 1, -1, "#FFFF14", true),
						new Cell(6, 1, -1, "#666600", true),
						new Cell(7, 1, -1, "#929591", true),
						new Cell(0, 2, -1, "#000066", true),
						new Cell(1, 2, -1, "#0000FF", true),
						new Cell(2, 2, -1, "#0066CC", true),
						new Cell(3, 2, -1, "#0080FF", true),
						new Cell(4, 2, -1, "#00FFFF", true),
						new Cell(5, 2, -1, "#C20078", true),
						new Cell(6, 2, -1, "#7E1E9C", true),
						new Cell(7, 2, -1, "#FFFFFF", true));

    gLayer = parseInt(localStorage["cubetube.currentlayer"]);
    gSelectedPieceIndex = parseInt(localStorage["cubetube.selectedpiece"]);
    gSelectedPieceArrayIndex = parseInt(localStorage["cubetube.selectedarray"]);
    gPickedColorIndex = parseInt(localStorage["cubetube.selectedcolorindex"]);
    gSelectedColor = localStorage["cubetube.selectedcolor"];

    drawCube();
    drawPicker();
    return true;
}

function newGame() {
	gPieces = new Array();
	for (var z=0; z<kBoardWidth; z++)
		for (var r=0; r<kBoardHeight; r++)
			for (var c=0; c<kBoardWidth; c++)
				gPieces.push(new Cell(r, c, -1, "#00000", false));

    gColors = new Array(new Cell(0, 0, -1, "#660000", true),
						new Cell(1, 0, -1, "#FF0000", true),
						new Cell(2, 0, -1, "#FF6666", true),
						new Cell(3, 0, -1, "#FF9999", true),
						new Cell(4, 0, -1, "#FFB266", true),
						new Cell(5, 0, -1, "#FF9933", true),
						new Cell(6, 0, -1, "#FF8000", true),
						new Cell(7, 0, -1, "#CC3300", true),
						new Cell(0, 1, -1, "#006600", true),
						new Cell(1, 1, -1, "#00FF00", true),
						new Cell(2, 1, -1, "#99FF99", true),
						new Cell(3, 1, -1, "#029386", true),
						new Cell(4, 1, -1, "#FFFF99", true),
						new Cell(5, 1, -1, "#FFFF14", true),
						new Cell(6, 1, -1, "#666600", true),
						new Cell(7, 1, -1, "#929591", true),
						new Cell(0, 2, -1, "#000066", true),
						new Cell(1, 2, -1, "#0000FF", true),
						new Cell(2, 2, -1, "#0066CC", true),
						new Cell(3, 2, -1, "#0080FF", true),
						new Cell(4, 2, -1, "#00FFFF", true),
						new Cell(5, 2, -1, "#C20078", true),
						new Cell(6, 2, -1, "#7E1E9C", true),
						new Cell(7, 2, -1, "#FFFFFF", true));

    gNumPieces = gPieces.length;
    gSelectedPieceIndex = -1;
    gSelectedPieceArrayIndex = -1;
    gLayer = 7;
    gGameInProgress = false;
	
    drawCube();
	drawPicker();
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

	/* voxel background image */
    gVoxelImage = new Image();
	gVoxelImage.src = "/static/images/voxel.png";
	gVoxelImage.onload = function() {
		// At this point, the image is fully loaded
		if (!resumeGame())
			newGame();
	};
}