#ifndef PIXEL_COLLISION_H_
#define PIXEL_COLLISION_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class PixelCollision {
public:
	static PixelCollision *getInstance(void);
	static void destroyInstance(void);

	bool collidesWithSprite(Sprite *sprite1, Sprite *sprite2, bool pp);
	bool collidesWithSprite(Sprite *sprite1, Sprite *sprite2);
	bool collidesWithPoint(Sprite *sprite, const Point &point);

private:
	class PixelReaderNode : public Node {
	public:
		static PixelReaderNode* create(const Point &readPoint);

		PixelReaderNode(const Point &readPoint);
		virtual ~PixelReaderNode(void);

		virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
		inline void reset();

		inline const Point &getReadPoint(void) const;
		inline void setReadPoint(const Point &readPoint);

		inline const Size &getReadSize(void) const;
		inline void setReadSize(const Size &readPoint);

		inline GLubyte *getBuffer(void);

	private:
		void onDraw(void);

		CustomCommand _readPixelCommand;
		Point _readPoint;
		Size _readSize;
		GLubyte *_buffer;
	};

	PixelCollision(void);
	virtual ~PixelCollision(void);

	Rect getIntersection(const Rect &r1, const Rect &r2) const;
	Point renderSprite(Sprite *sprite, CustomCommand &command, bool red);
	void resetSprite(Sprite *sprite, const Point &oldPosition);

	// Singleton
	static PixelCollision *s_instance;

	GLProgram *_glProgram;
	RenderTexture *_rt;
	PixelReaderNode *_pixelReader;
};

// Inline methods
inline void PixelCollision::PixelReaderNode::reset(void) {
	memset(_buffer, 0, 4 * _readSize.width * _readSize.height);
}

inline const Point &PixelCollision::PixelReaderNode::getReadPoint(void) const {
	return _readPoint;
}

inline void PixelCollision::PixelReaderNode::setReadPoint(const Point &readPoint) {
	_readPoint = readPoint;
}

inline const Size &PixelCollision::PixelReaderNode::getReadSize(void) const {
	return _readSize;
}

inline void PixelCollision::PixelReaderNode::setReadSize(const Size &readSize) {
	if (_readSize.width * _readSize.height < readSize.width * readSize.height) {
		free(_buffer);
		_buffer = (GLubyte*)malloc(4 * readSize.width * readSize.height);
	}
	_readSize = readSize;
}

inline GLubyte *PixelCollision::PixelReaderNode::getBuffer(void) {
	return _buffer;
}

#endif /* PIXEL_COLLISION_H_ */