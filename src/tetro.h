#ifndef TETRO_H
#define TETRO_H

#include <algorithm>
#include <array>
#include <deque>
#include <random>
#include <iterator>
#include <memory>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <cstddef>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "block.h"
#include "movements.h"

class TetroFactoryBase;
class GhostTetro;

class Tetro : public sf::Drawable {
	friend GhostTetro;
	friend TetroFactoryBase;

	public:

	typedef std::vector<TetroBlock> BlockList;
	typedef BlockList::const_iterator const_iterator;
	typedef std::pair<int8_t, int8_t> WallKickOffset;
	typedef std::pair<int8_t, int8_t> WallKickTranslation;
	typedef std::vector<WallKickOffset> WallKickOffsetRow;

	const const_iterator begin() const {
		return this->_blocks.begin();
	};

	const const_iterator end() const {
		return this->_blocks.end();
	};

	// When you see a white block that's not a tetromino, you
	// now something is not quite right.
	Tetro()
		: Tetro(sf::Color::White, {
				{-1,  1},
					{-1,  0}, {0, 0}, {1, 0}, {2, 0}
			}, &jlstz_offsets)
	{}

	Tetro(const Tetro& t)
		: _col(t._col), _row(t._row), _orntn(t._orntn)
		, _wall_kick_offsets(t._wall_kick_offsets), _c(t._c),
			_blocks(t._blocks)
	{}

	Tetro& operator=(const Tetro& t) {
		_col = t._col;
		_row = t._row;
		_orntn = t._orntn;
		_c = t._c;
		_blocks = t._blocks;
		_wall_kick_offsets = t._wall_kick_offsets;

		return *this;
	}

	Tetro& operator= (Tetro&& t) {
		_col = std::move(t._col);
		_row = std::move(t._row);
		_orntn = std::move(t._orntn);
		_c = std::move(t._c);
		_blocks = std::move(t._blocks);
		_wall_kick_offsets = t._wall_kick_offsets;
		t._wall_kick_offsets = nullptr;

		return *this;
	}

	bool operator== (const Tetro& other) const {
		return std::equal(_blocks.begin(), _blocks.end(), other._blocks.begin());
	}

	uint8_t getColumn() const;
	uint8_t getRow() const;
	int8_t getFinalX(const TetroBlock&) const;
	int8_t getFinalY(const TetroBlock&) const;
	sf::Color getColor() const;
	WallKickOffset getWallKickOffset(uint8_t);

	WallKickTranslation rotate(rotation, uint8_t = 0);
	void translate(int8_t, int8_t);
	void move(direction);

	static const sf::Color Cyan;
	static const sf::Color Yellow;
	static const sf::Color Purple;
	static const sf::Color Green;
	static const sf::Color Red;
	static const sf::Color Blue;
	static const sf::Color Orange;

	static WallKickOffsetRow jlstz_offsets[4];
	static WallKickOffsetRow i_offsets[4];
	static WallKickOffsetRow o_offsets[4];

  protected:
	Tetro(sf::Color, BlockList, WallKickOffsetRow (*)[4] = &jlstz_offsets);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

  private:
	WallKickTranslation getWallKickTranslation(rotation, uint8_t);

	uint8_t _col;
	uint8_t _row;
	orientation _orntn;
	WallKickOffsetRow (*_wall_kick_offsets)[4];

	sf::Color _c;
	BlockList _blocks;
};

class TetroFactoryBase {
  public:
	typedef Tetro (* TetroCreator)();

	TetroFactoryBase(const TetroFactoryBase& other) : _buf(other._buf)
	{}

	TetroFactoryBase(size_t n) : _buf(n)
	{}

	static Tetro createI() {
		return Tetro(Tetro::Cyan, {
			{-1, 0}, { 0, 0}, { 1, 0}, { 2, 0}
		}, &Tetro::i_offsets);
	}

	static Tetro createJ() {
		return Tetro(Tetro::Blue, {
			{-1,  1},
			{-1,  0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createL() {
		return Tetro(Tetro::Orange, {
			                   {1, 1},
			{-1,  0}, {0,  0}, {1, 0}
		});
	}

	static Tetro createO() {
		return Tetro(Tetro::Yellow, {
			{0, 1}, {1, 1},
			{0, 0}, {1, 0}
		}, &Tetro::o_offsets);
	}

	static Tetro createS() {
		return Tetro(Tetro::Green, {
			         {0, 1}, {1,  1},
			{-1, 0}, {0, 0}
		});
	}

	static Tetro createT() {
		return Tetro(Tetro::Purple, {
			         {0, 1},
			{-1, 0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createZ() {
		return Tetro(Tetro::Red, {
			{-1, 1}, {0, 1},
			         {0, 0}, {1, 0}
		});
	}

	Tetro next() {
		Tetro&& last = this->operator()();
		Tetro next = _buf.front();
		_buf.pop_front();
		_buf.push_back(last);
		return next;
	}

	virtual bool operator== (const TetroFactoryBase& other) const {
		return std::equal(other._buf.begin(), other._buf.end(), this->_buf.begin());
	}

  protected:
	virtual Tetro operator() () = 0;
	virtual Tetro& operator[] (int n) const {
		return _buf[n];
	}

	mutable std::deque<Tetro> _buf;
};

template <class TetroFactory, class Tet, std::size_t n>
class TetroIterator;

template <class TetroFactory, class Tet, std::size_t n>
class TetroIterator : public boost::iterator_facade <
	TetroIterator<TetroFactory, Tet, n>, // Derived
	Tet,                                 // Value
	boost::forward_traversal_tag         // CategoryOrTraversal
	>
{
  private:
	struct enabler {};

  public:
	TetroIterator()
		: TetroIterator(new TetroFactory())
	{}

	explicit TetroIterator(TetroFactory& tf)
		: _tf(tf)
		, _pos(0)
	{}

	template <class OtherFactory, class OtherTet, size_t otherSize>
	TetroIterator(
								TetroIterator<OtherFactory, OtherTet, otherSize> const& other
								, typename boost::enable_if<
								boost::is_convertible<OtherTet, Tet>
								, enabler
								>::type = enabler()
								)
		: _tf(other._tf)
		, _pos(other._pos)
	{}

  private:
	friend class boost::iterator_core_access;
	template <class, class, size_t> friend class TetroIterator;

	void increment() {
		++_pos;
	}

	template <class OtherFactory, class OtherTet, std::size_t otherSize>
	bool equal(TetroIterator<OtherFactory, OtherTet, otherSize> otherIt) const {
		return _pos == otherIt._pos && _tf == otherIt._tf;
	}

	Tet& dereference() const {
		return _tf[_pos];
	}

	TetroFactory& _tf;
	uint8_t _pos;
};

class GuidelineTetroFactory : public TetroFactoryBase {
  public:
	GuidelineTetroFactory()
		: TetroFactoryBase(3)
		,	_random_generator(std::random_device{}())
		, it(_creators.end()) {
		// Fill buffer.
		std::generate_n(_buf.begin(), 3, [this]() -> Tetro { return this->operator()(); });
	}
	GuidelineTetroFactory(const GuidelineTetroFactory& gt)
		: TetroFactoryBase(gt), _random_generator(gt._random_generator)
	{
		_creators = gt._creators;
		it = _creators.begin();
		std::advance(it, std::distance(gt._creators.begin(), (std::array<TetroCreator, 7>::const_iterator) gt.it));
	}

	typedef TetroIterator<GuidelineTetroFactory, Tetro, 3> iterator;
	typedef TetroIterator<const GuidelineTetroFactory, const Tetro, 3> const_iterator;

	const_iterator begin() const {
		return const_iterator(*this);
	}

	iterator begin() {
		return iterator(*this);
	}

	iterator end() {
		iterator&& begin = this->begin();
		std::advance(begin, 3);
		return begin;
	}

	const_iterator end() const {
		const_iterator&& begin = this->begin();
		std::advance(begin, 3);
		return begin;
	}

  protected:
	Tetro operator() () {
		if (it == _creators.end()) {
			std::shuffle(_creators.begin(), _creators.end(), _random_generator);
			it = _creators.begin();
		}
		return (*it++)();
	}

  private:
	friend class TetroIterator<GuidelineTetroFactory, Tetro, 3>;
	friend class TetroIterator<const GuidelineTetroFactory, const Tetro, 3>;

	std::array<TetroCreator, 7> _creators {{
		&createI, &createJ, &createL, &createO, &createS, &createT, &createZ
	}};
	std::mt19937 _random_generator;
	std::array<TetroCreator, 7>::iterator it;
};

typedef GuidelineTetroFactory::iterator GuidelineTetroIterator;
typedef GuidelineTetroFactory::const_iterator GuidelineTetroConstIterator;

#endif /* TETRO_H */
