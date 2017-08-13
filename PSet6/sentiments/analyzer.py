import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.pos = set()
        self.neg = set()
        
        positiveFile = open(positives, "r")
        for line in positiveFile:
            if line[0]==";" or line[0]=="\n":
                continue
            self.pos.add(line.rstrip("\n"))
        positiveFile.close()
        
        negativeFile = open(negatives, "r")
        for line in negativeFile:
            if line[0]==";" or line[0]=="\n":
                continue
            self.neg.add(line.rstrip("\n"))
        positiveFile.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        score = 0
        for word in text.split():
            if word.lower() in self.pos:
                score+=1
            elif word.lower() in self.neg:
                score+=-1
        
        return score
